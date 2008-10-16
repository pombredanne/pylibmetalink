#ifndef LIBMETALINK_H
#define LIBMETALINK_H 1

#include <Python.h>
#include <structmember.h>
#include <metalink/metalink_parser.h>

typedef struct
{
	PyObject_HEAD
	PyObject *files;
	PyObject *arg;
	metalink_t *metalink;
} cMetalinkObject;

PyObject *libmetalinkException;

static uint8_t cMetalink_createPyObject(cMetalinkObject *self, __attribute__((unused)) PyObject *args);

#endif /* LIBMETALINK_H */
