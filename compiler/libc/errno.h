#ifndef LIBC_ERRNO_H
#define LIBC_ERRNO_H
int* _get_errno_address();
#define errno (*_get_errno_address())
#endif