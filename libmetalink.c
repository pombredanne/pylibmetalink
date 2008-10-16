#include "libmetalink.h"

static const char __author__[] =
"The libmetalink python module was written by:\n\
\n\
Per Øyvind Karlsen <peroyvind@mandriva.org>\n";

PyDoc_STRVAR(cMetalink_init__doc__,
"metalink(arg) -- Initializes a metalink object.\n\
The arg argument must be either a file, string or read-only buffer\n");

static int8_t
cMetalink_init(cMetalinkObject *self, PyObject *args)
{
	metalink_error_t ret;
	PyObject *arg;
	if (!PyArg_ParseTuple(args, "O:metalink", &arg))
		return -1;

	if(PyFile_Check(arg))
	{
		if(PyFile_AsFile(arg))
			ret = metalink_parse_fp(PyFile_AsFile(arg), &self->metalink);
		else
			return -1;
	}
	else if(PyString_Check(arg))
	{
		char *buf;
		Py_ssize_t len;
		if(!PyString_AsStringAndSize(arg, &buf, &len))
			ret = metalink_parse_memory(buf, len, &self->metalink);
		else
			return -1;
	}
	else
	{
		PyErr_Format(PyExc_TypeError, "%s constructor argument must be either a file,"
				"string or read-only buffer, not %s",
				((PyTypeObject*)PyObject_Type((PyObject*)self))->tp_name,
				((PyTypeObject*)PyObject_Type(arg))->tp_name);
		return -1;
	}


	if(ret) {
		PyErr_Format(libmetalinkException, "Error %d returned from libmetalink "
				"trying to parse document.", ret);
		return -1;
	}

	self->files = PyList_New(0);
	metalink_file_t** file = self->metalink->files;

	while(*file) {
		PyObject *fileDict = PyDict_New();
		PyDict_SetItem(fileDict, PyString_FromString("filename"),
				PyString_FromString((*file)->name));

		PyDict_SetItem(fileDict, PyString_FromString("size"),
				PyInt_FromLong((*file)->size));
		PyDict_SetItem(fileDict, PyString_FromString("os"),
				PyString_FromString((*file)->os));
		if((*file)->checksums) {
			PyObject *checksumDict = PyDict_New();
			metalink_checksum_t** checksums = (*file)->checksums;
			while(*checksums) {
				PyDict_SetItem(checksumDict, PyString_FromString((*checksums)->type),
						PyString_FromString((*checksums)->hash));
				++checksums;
			}
			PyDict_SetItem(fileDict, PyString_FromString("checksums"),
					checksumDict);
		}
		if((*file)->chunk_checksum) {
			metalink_piece_hash_t** piece_hashes;
			PyObject *chunk_checksumDict = PyDict_New();
			PyObject *piece_hashList = PyList_New(0);

			PyDict_SetItem(chunk_checksumDict, PyString_FromString("size"),
					PyInt_FromLong((*file)->chunk_checksum->length));
			PyDict_SetItem(chunk_checksumDict, PyString_FromString("type"),
					PyString_FromString((*file)->chunk_checksum->type));

			piece_hashes = (*file)->chunk_checksum->piece_hashes;
			while(*piece_hashes) {
				PyObject *piece_hashTuple = Py_BuildValue("(is)", (*piece_hashes)->piece, (*piece_hashes)->hash);
				PyList_Append(piece_hashList, piece_hashTuple);
				++piece_hashes;
			}
			PyDict_SetItem(chunk_checksumDict, PyString_FromString("piece_hashes"),
					piece_hashList);
			PyDict_SetItem(fileDict, PyString_FromString("chunk_checksum"),
					chunk_checksumDict);
		}
		if((*file)->resources) {
			metalink_resource_t** resources;
			PyObject *resourceList = PyList_New(0);
			resources = (*file)->resources;
			while(*resources) {
				PyObject *resourceDict = PyDict_New();
				PyDict_SetItem(resourceDict, PyString_FromString("type"),
						PyString_FromString((*resources)->type));
				if((*resources)->location)
					PyDict_SetItem(resourceDict, PyString_FromString("location"),
							PyString_FromString((*resources)->location));
				if((*resources)->preference)
					PyDict_SetItem(resourceDict, PyString_FromString("preference"),
							PyInt_FromLong((*resources)->preference));
				if((*resources)->url)
					PyDict_SetItem(resourceDict, PyString_FromString("url"),
							PyString_FromString((*resources)->url));
				PyList_Append(resourceList, resourceDict);
				++resources;
			}
			PyDict_SetItem(fileDict, PyString_FromString("resources"),
					resourceList);
		}

		PyList_Append(self->files, fileDict);
		++file;
	}

	return 0;
}


static PyObject *
cMetalinkObject_new(PyTypeObject *type, __attribute__((unused)) PyObject *args, __attribute__((unused)) PyObject *kwargs)
{
	cMetalinkObject *self;
	self = (cMetalinkObject *)type->tp_alloc(type, 0);
	if(self == NULL){
		Py_DECREF(self);
		return NULL;
	}

	return (PyObject *)self;
}

static void
cMetalink_dealloc(cMetalinkObject *self)
{
	Py_XDECREF(self->files);
	Py_XDECREF(self->arg);

	if(self->metalink)
		delete_metalink(self->metalink);

	self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
cMetalink_get_files(cMetalinkObject *self, __attribute__((unused))void *closure)
{
	return self->files;
}

static PyGetSetDef cMetalink_getset[] = {
	{"files", (getter)cMetalink_get_files, NULL,
		"List of files in metalink", NULL},
	{NULL, NULL, NULL, NULL, NULL}	/* Sentinel */
};

	PyTypeObject cMetalink_Type = {
		PyObject_HEAD_INIT(NULL)
		0,						/*ob_size*/
		"cMetalink.metalink",				/*tp_name*/
		sizeof(cMetalinkObject),			/*tp_basicsize*/
		0,						/*tp_itemsize*/
		(destructor)cMetalink_dealloc,			/*tp_dealloc*/
		0,						/*tp_print*/
		0,						/*tp_getattr*/
		0,						/*tp_setattr*/
		0,						/*tp_compare*/
		0,						/*tp_repr*/
		0,						/*tp_as_number*/
		0,						/*tp_as_sequence*/
		0,						/*tp_as_mapping*/
		0,						/*tp_hash*/
		0,						/*tp_call*/
		0,						/*tp_str*/
		PyObject_GenericGetAttr,			/*tp_getattro*/
		PyObject_GenericSetAttr,			/*tp_setattro*/
		0,						/*tp_as_buffer*/
		Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,		/*tp_flags*/
		cMetalink_init__doc__,         			/*tp_doc*/
		0,						/*tp_traverse*/
		0,						/*tp_clear*/
		0,						/*tp_richcompare*/
		0,						/*tp_weaklistoffset*/
		0,						/*tp_iter*/
		0,						/*tp_iternext*/
		0,						/*tp_methods*/
		0,						/*tp_members*/
		cMetalink_getset,				/*tp_getset*/
		0,						/*tp_base*/
		0,						/*tp_dict*/
		0,						/*tp_descr_get*/
		0,						/*tp_descr_set*/
		0,						/*tp_dictoffset*/
		(initproc)cMetalink_init,			/*tp_init*/
		PyType_GenericAlloc,				/*tp_alloc*/
		cMetalinkObject_new,				/*tp_new*/
		0,						/*tp_free*/
		0,						/*tp_is_gc*/
		0,						/*tp_bases*/
		0,						/*tp_mro*/
		0,						/*tp_cache*/
		0,						/*tp_subclasses*/
		0,						/*tp_weaklist*/
		0						/*tp_del*/
	};

static PyMethodDef cMetalink_methods[] = {
	{0, 0, 0, 0}
};

PyDoc_STRVAR(cMetalink_module_documentation,
		"The python cMetalink module provides an interface for the libmetalink\n\
		library.");

/* declare function before defining it to avoid compile warnings */
PyMODINIT_FUNC initcMetalink(void);
PyMODINIT_FUNC
initcMetalink(void)
{
	PyObject *m;
	if (PyType_Ready(&cMetalink_Type) < 0)
		return;
	m = Py_InitModule3("cMetalink", cMetalink_methods,
			cMetalink_module_documentation);
	if (m == NULL)
		return;


	libmetalinkException = PyErr_NewException("cMetalink.Error", NULL, NULL);
	if (libmetalinkException != NULL) {
		Py_INCREF(libmetalinkException);
		PyModule_AddObject(m, "Error", libmetalinkException);
	}

	Py_INCREF(&cMetalink_Type);
	PyModule_AddObject(m, "metalink", (PyObject *)&cMetalink_Type);

	PyModule_AddObject(m, "__author__", PyString_FromString(__author__));

	PyModule_AddStringConstant(m, "__version__", VERSION);
}

