#ifndef LIBMETALINK_H
#define LIBMETALINK_H 1

#include <Python.h>
#include <structmember.h>

typedef struct
{
        PyObject_HEAD
} cMetalinkObject;

PyObject *shortException, *unknownException;

#endif /* LIBMETALINK_H */
