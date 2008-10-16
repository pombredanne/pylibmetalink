#ifndef LIBMETALINK_H
#define LIBMETALINK_H 1

#include <Python.h>
#include <structmember.h>
#include <metalink/metalink_parser.h>

typedef struct
{
	PyObject_HEAD
	PyObject *fp;	
	PyObject *files;
	metalink_t *metalink;
} cMetalinkObject;

PyObject *libmetalinkException;

#endif /* LIBMETALINK_H */
