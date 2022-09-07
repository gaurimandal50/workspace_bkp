Three ways of creating and handling read write of proc file are as follows:-
https://tldp.org/LDP/lkmpg/2.6/html/lkmpg.html#AEN687

5.2. Read and Write a /proc File

We have seen a very simple example for a /proc file where we only read the file /proc/helloworld. It's also possible to write in a /proc file. It works the same way as read, a function is called when the /proc file is written. But there is a little difference with read, data comes from user, so you have to import data from user space to kernel space (with copy_from_user or get_user)

The reason for copy_from_user or get_user is that Linux memory (on Intel architecture, it may be different under some other processors) is segmented. This means that a pointer, by itself, does not reference a unique location in memory, only a location in a memory segment, and you need to know which memory segment it is to be able to use it. There is one memory segment for the kernel, and one for each of the processes.

The only memory segment accessible to a process is its own, so when writing regular programs to run as processes, there's no need to worry about segments. When you write a kernel module, normally you want to access the kernel memory segment, which is handled automatically by the system. However, when the content of a memory buffer needs to be passed between the currently running process and the kernel, the kernel function receives a pointer to the memory buffer which is in the process segment. The put_user and get_user macros allow you to access that memory. These functions handle only one caracter, you can handle several caracters with copy_to_user and copy_from_user. As the buffer (in read or write function) is in kernel space, for write function you need to import data because it comes from user space, but not for the read function because data is already in kernel space.

Example 5-2. procfs2.c

/**
 *  procfs2.c -  create a "file" in /proc
 *
 */

#include <linux/module.h>	/* Specifically, a module */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <asm/uaccess.h>	/* for copy_from_user */

#define PROCFS_MAX_SIZE		1024
#define PROCFS_NAME 		"buffer1k"

/**
 * This structure hold information about the /proc file
 *
 */
static struct proc_dir_entry *Our_Proc_File;

/**
 * The buffer used to store character for this module
 *
 */
static char procfs_buffer[PROCFS_MAX_SIZE];

/**
 * The size of the buffer
 *
 */
static unsigned long procfs_buffer_size = 0;

/** 
 * This function is called then the /proc file is read
 *
 */
int 
procfile_read(char *buffer,
	      char **buffer_location,
	      off_t offset, int buffer_length, int *eof, void *data)
{
	int ret;
	
	printk(KERN_INFO "procfile_read (/proc/%s) called\n", PROCFS_NAME);
	
	if (offset > 0) {
		/* we have finished to read, return 0 */
		ret  = 0;
	} else {
		/* fill the buffer, return the buffer size */
		memcpy(buffer, procfs_buffer, procfs_buffer_size);
		ret = procfs_buffer_size;
	}

	return ret;
}

/**
 * This function is called with the /proc file is written
 *
 */
int procfile_write(struct file *file, const char *buffer, unsigned long count,
		   void *data)
{
	/* get buffer size */
	procfs_buffer_size = count;
	if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}
	
	/* write data to the buffer */
	if ( copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
		return -EFAULT;
	}
	
	return procfs_buffer_size;
}

/**
 *This function is called when the module is loaded
 *
 */
int init_module()
{
	/* create the /proc file */
	Our_Proc_File = create_proc_entry(PROCFS_NAME, 0644, NULL);
	
	if (Our_Proc_File == NULL) {
		remove_proc_entry(PROCFS_NAME, &proc_root);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
			PROCFS_NAME);
		return -ENOMEM;
	}

	Our_Proc_File->read_proc  = procfile_read;
	Our_Proc_File->write_proc = procfile_write;
	Our_Proc_File->owner 	  = THIS_MODULE;
	Our_Proc_File->mode 	  = S_IFREG | S_IRUGO;
	Our_Proc_File->uid 	  = 0;
	Our_Proc_File->gid 	  = 0;
	Our_Proc_File->size 	  = 37;

	printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);	
	return 0;	/* everything is ok */
}

/**
 *This function is called when the module is unloaded
 *
 */
void cleanup_module()
{
	remove_proc_entry(PROCFS_NAME, &proc_root);
	printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
}

5.3. Manage /proc file with standard filesystem

We have seen how to read and write a /proc file with the /proc interface. But it's also possible to manage /proc file with inodes. The main interest is to use advanced function, like permissions.

In Linux, there is a standard mechanism for file system registration. Since every file system has to have its own functions to handle inode and file operations[9], there is a special structure to hold pointers to all those functions, struct inode_operations, which includes a pointer to struct file_operations. In /proc, whenever we register a new file, we're allowed to specify which struct inode_operations will be used to access to it. This is the mechanism we use, a struct inode_operations which includes a pointer to a struct file_operations which includes pointers to our procfs_read and procfs_write functions.

Another interesting point here is the module_permission function. This function is called whenever a process tries to do something with the /proc file, and it can decide whether to allow access or not. Right now it is only based on the operation and the uid of the current user (as available in current, a pointer to a structure which includes information on the currently running process), but it could be based on anything we like, such as what other processes are doing with the same file, the time of day, or the last input we received.

It's important to note that the standard roles of read and write are reversed in the kernel. Read functions are used for output, whereas write functions are used for input. The reason for that is that read and write refer to the user's point of view --- if a process reads something from the kernel, then the kernel needs to output it, and if a process writes something to the kernel, then the kernel receives it as input.

Example 5-3. procfs3.c

/* 
 *  procfs3.c -  create a "file" in /proc, use the file_operation way
 *  		to manage the file.
 */
 
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/proc_fs.h>	/* Necessary because we use proc fs */
#include <asm/uaccess.h>	/* for copy_*_user */

#define PROC_ENTRY_FILENAME 	"buffer2k"
#define PROCFS_MAX_SIZE 	2048

/**
 * The buffer (2k) for this module
 *
 */
static char procfs_buffer[PROCFS_MAX_SIZE];

/**
 * The size of the data hold in the buffer
 *
 */
static unsigned long procfs_buffer_size = 0;

/**
 * The structure keeping information about the /proc file
 *
 */
static struct proc_dir_entry *Our_Proc_File;

/**
 * This funtion is called when the /proc file is read
 *
 */
static ssize_t procfs_read(struct file *filp,	/* see include/linux/fs.h   */
			     char *buffer,	/* buffer to fill with data */
			     size_t length,	/* length of the buffer     */
			     loff_t * offset)
{
	static int finished = 0;

	/* 
	 * We return 0 to indicate end of file, that we have
	 * no more information. Otherwise, processes will
	 * continue to read from us in an endless loop. 
	 */
	if ( finished ) {
		printk(KERN_INFO "procfs_read: END\n");
		finished = 0;
		return 0;
	}
	
	finished = 1;
		
	/* 
	 * We use put_to_user to copy the string from the kernel's
	 * memory segment to the memory segment of the process
	 * that called us. get_from_user, BTW, is
	 * used for the reverse. 
	 */
	if ( copy_to_user(buffer, procfs_buffer, procfs_buffer_size) ) {
		return -EFAULT;
	}

	printk(KERN_INFO "procfs_read: read %lu bytes\n", procfs_buffer_size);

	return procfs_buffer_size;	/* Return the number of bytes "read" */
}

/*
 * This function is called when /proc is written
 */
static ssize_t
procfs_write(struct file *file, const char *buffer, size_t len, loff_t * off)
{
	if ( len > PROCFS_MAX_SIZE )	{
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}
	else	{
		procfs_buffer_size = len;
	}
	
	if ( copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
		return -EFAULT;
	}

	printk(KERN_INFO "procfs_write: write %lu bytes\n", procfs_buffer_size);
	
	return procfs_buffer_size;
}

/* 
 * This function decides whether to allow an operation
 * (return zero) or not allow it (return a non-zero
 * which indicates why it is not allowed).
 *
 * The operation can be one of the following values:
 * 0 - Execute (run the "file" - meaningless in our case)
 * 2 - Write (input to the kernel module)
 * 4 - Read (output from the kernel module)
 *
 * This is the real function that checks file
 * permissions. The permissions returned by ls -l are
 * for referece only, and can be overridden here.
 */

static int module_permission(struct inode *inode, int op, struct nameidata *foo)
{
	/* 
	 * We allow everybody to read from our module, but
	 * only root (uid 0) may write to it 
	 */
	if (op == 4 || (op == 2 && current->euid == 0))
		return 0;

	/* 
	 * If it's anything else, access is denied 
	 */
	return -EACCES;
}

/* 
 * The file is opened - we don't really care about
 * that, but it does mean we need to increment the
 * module's reference count. 
 */
int procfs_open(struct inode *inode, struct file *file)
{
	try_module_get(THIS_MODULE);
	return 0;
}

/* 
 * The file is closed - again, interesting only because
 * of the reference count. 
 */
int procfs_close(struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);
	return 0;		/* success */
}

static struct file_operations File_Ops_4_Our_Proc_File = {
	.read 	 = procfs_read,
	.write 	 = procfs_write,
	.open 	 = procfs_open,
	.release = procfs_close,
};

/* 
 * Inode operations for our proc file. We need it so
 * we'll have some place to specify the file operations
 * structure we want to use, and the function we use for
 * permissions. It's also possible to specify functions
 * to be called for anything else which could be done to
 * an inode (although we don't bother, we just put
 * NULL). 
 */

static struct inode_operations Inode_Ops_4_Our_Proc_File = {
	.permission = module_permission,	/* check for permissions */
};

/* 
 * Module initialization and cleanup 
 */
int init_module()
{
	/* create the /proc file */
	Our_Proc_File = create_proc_entry(PROC_ENTRY_FILENAME, 0644, NULL);
	
	/* check if the /proc file was created successfuly */
	if (Our_Proc_File == NULL){
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
		       PROC_ENTRY_FILENAME);
		return -ENOMEM;
	}
	
	Our_Proc_File->owner = THIS_MODULE;
	Our_Proc_File->proc_iops = &Inode_Ops_4_Our_Proc_File;
	Our_Proc_File->proc_fops = &File_Ops_4_Our_Proc_File;
	Our_Proc_File->mode = S_IFREG | S_IRUGO | S_IWUSR;
	Our_Proc_File->uid = 0;
	Our_Proc_File->gid = 0;
	Our_Proc_File->size = 80;

	printk(KERN_INFO "/proc/%s created\n", PROC_ENTRY_FILENAME);

	return 0;	/* success */
}

void cleanup_module()
{
	remove_proc_entry(PROC_ENTRY_FILENAME, &proc_root);
	printk(KERN_INFO "/proc/%s removed\n", PROC_ENTRY_FILENAME);
}

Still hungry for procfs examples? Well, first of all keep in mind, there are rumors around, claiming that procfs is on it's way out, consider using sysfs instead. Second, if you really can't get enough, there's a highly recommendable bonus level for procfs below linux/Documentation/DocBook/ . Use make help in your toplevel kernel directory for instructions about how to convert it into your favourite format. Example: make htmldocs . Consider using this mechanism, in case you want to document something kernel related yourself.

5.4. Manage /proc file with seq_file  //contd
