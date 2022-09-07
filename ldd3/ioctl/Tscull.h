#ifndef _TSCULL_H_
#define _TSCULL_H_ 1

#define SCULL_SET_PARAM _IOW(major, 0, int *)
#define SCULL_GET_PARAM _IOR(major, 1, int*)
#define SCULL_SETGET_PARAM _IOWR(major, 2, int *)

#endif /*_TSCULL_H_*/
