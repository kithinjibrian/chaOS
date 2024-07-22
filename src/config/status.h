#ifndef STATUS_H
#define STATUS_H

/**
 * every component is right
 */
#define SHEAROS_ALL_OK 0
/**
 * error - input/ouptut stream
 */
#define EIO 1
/**
 * error - invalid argument
 */
#define EINVARG 2
/**
 * error - insufficient memory
 */
#define ENOMEM 3
/**
 * error - bad path
 */
#define EBADPATH 4
/**
 * error - not our component
 */
#define EFSNOTUS 5
/**
 * error - read only attribute
 */
#define ERDONLY 6
/**
 * error - unimplemented
 */
#define EUNIMP 7
/**
 * error - is taken
 */
#define EISTKN 8
/**
 * error - invalid format
 */
#define EINFORMAT 9

#endif