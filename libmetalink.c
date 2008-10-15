#include "libmetalink.h"

#include <metalink/metalink_parser.h>

static const char __author__[] =
"The libmetalink python module was written by:\n\
\n\
    Per Øyvind Karlsen <peroyvind@mandriva.org>\n\
";

PyDoc_STRVAR(cMetalink_init__doc__,
"");

static int8_t
cMetalink_init(cMetalinkObject *self, PyObject *args)
{
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

PyDoc_STRVAR(cMetalink_parse__doc__,
"");
static PyObject*
cMetalink_parse(cMetalinkObject *self, PyObject *args)
{
}

static PyMethodDef cMetalinkObject_methods[] = {
    	{"parse", (PyCFunction)cMetalink_parse, METH_VARARGS | METH_KEYWORDS,
		cMetalink_parse__doc__},
	
	{0, 0, 0, 0}
};

static void
cMetalink_dealloc(cMetalinkObject *self)
{
	self->ob_type->tp_free((PyObject*)self);
}

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
	0,						/*tp_getattro*/
	0,						/*tp_setattro*/
	0,						/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,		/*tp_flags*/
	cMetalink_init__doc__,         		/*tp_doc*/
	0,						/*tp_traverse*/
	0,						/*tp_clear*/
	0,						/*tp_richcompare*/
	0,						/*tp_weaklistoffset*/
	0,						/*tp_iter*/
	0,						/*tp_iternext*/
	cMetalinkObject_methods,			/*tp_methods*/
	0,						/*tp_members*/
	0,						/*tp_getset*/
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

static PyMethodDef cmetalink_methods[] = {
	{0, 0, 0, 0}
};

PyDoc_STRVAR(cmetalink_module_documentation,
"The python cMetalink module provides an interface for the libmetalink\n\
library.");

/* declare function before defining it to avoid compile warnings */
PyMODINIT_FUNC initcmetalink(void);
PyMODINIT_FUNC
initcmetalink(void)
{
    PyObject *m;
    if (PyType_Ready(&cMetalink_Type) < 0)
 	    return;
    m = Py_InitModule3("cMetalink", cmetalink_methods,
		       cmetalink_module_documentation);
    if (m == NULL)
		return;
    shortException = PyErr_NewException("metalink.ShortException", NULL, NULL);
    if (shortException != NULL) {
        Py_INCREF(shortException);
		PyModule_AddObject(m, "ShortException", shortException);
    }

    unknownException = PyErr_NewException("metalink.UnknownException", NULL, NULL);
    if (unknownException != NULL) {
        Py_INCREF(unknownException);
		PyModule_AddObject(m, "UnknownException", unknownException);
    }

    Py_INCREF(&cMetalink_Type);
    PyModule_AddObject(m, "metalink", (PyObject *)&cMetalink_Type);

    PyModule_AddObject(m, "__author__", PyString_FromString(__author__));

    PyModule_AddStringConstant(m, "__version__", VERSION);
}

