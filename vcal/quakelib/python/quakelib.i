%module quakelib
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%{
#include "QuakeLib.h"
#include "QuakeLibOkada.h"

using namespace quakelib;
%}

// Ignore the reader/writer classes since the user shouldn't be using these anyway
%ignore quakelib::EQSimFileReader;
%ignore quakelib::EQSimFileWriter;

// Create vector templates for iterating through lists
%template(EQSimEventSummaryList) std::vector<quakelib::EQSimEventSummary>;
%template(EQSimEventSlipList) std::vector<quakelib::EQSimEventSlipMap>;
%template(LatLonDepthPointList) std::vector<quakelib::LatLonDepth>;

%include "QuakeLibUtil.h"
%include "QuakeLib.h"
%include "QuakeLibOkada.h"

// Create aliases for 2D and 3D vector templates
%template(Vec2) quakelib::Vec<2>;
%template(Vec3) quakelib::Vec<3>;
%extend quakelib::Vec<2> {
	double __getitem__(unsigned int i) throw(std::out_of_range) { return (*$self)[i]; };
	void __setitem__(unsigned int i, double new_val) throw(std::out_of_range) { (*$self)[i] = new_val; };
};

%extend quakelib::Vec<3> {
	double __getitem__(unsigned int i) throw(std::out_of_range) { return (*$self)[i]; };
	void __setitem__(unsigned int i, double new_val) throw(std::out_of_range) { (*$self)[i] = new_val; };
};

%template(TensorRow3) quakelib::TensorRow<3>;
%template(Tensor33) quakelib::Tensor<3,3>;

%extend quakelib::TensorRow<3> {
	double __getitem__(unsigned int i) throw(std::out_of_range) { return (*$self)[i]; };
	void __setitem__(unsigned int i, double new_val) throw(std::out_of_range) { (*$self)[i] = new_val; };
};

%extend quakelib::Tensor<3,3> {
	TensorRow<3> __getitem__(unsigned int i) throw(std::out_of_range) { return (*$self)[i]; };
	void __setitem__(unsigned int i, TensorRow<3> new_val) throw(std::out_of_range) { (*$self)[i] = new_val; };
};

%template(RectBound2) quakelib::RectBound<2>;
%template(RectBound3) quakelib::RectBound<3>;

%template(Octree2) quakelib::Octree<2>;
%template(Octree3) quakelib::Octree<3>;

// TODO: check all these for potential buffer overflows
// Python __str__ and __repr__ functions for QuakeLib classes
%extend quakelib::LatLonDepth {
	char *__str__(void) {
		static char			tmp[1024];
		std::stringstream	ss;
		std::string			res;
		ss << (*$self);
		res = ss.str();
		res.resize(1023);
		sprintf(tmp, "%s", res.c_str());
		return tmp;
	}
	
	char *__repr__(void) {
		static char tmp[1024];
		sprintf(tmp, "quakelib.LatLonDepth(%g,%g,%g)", $self->lat(), $self->lon(), $self->altitude());
		return tmp;
	}
};

%extend quakelib::Vec<2> {
	char *__str__(void) {
		static char tmp[1024];
		std::stringstream	ss;
		std::string			res;
		ss << (*$self);
		res = ss.str();
		res.resize(1023);
		sprintf(tmp, "%s", res.c_str());
		return tmp;
	}
	
	char *__repr__(void) {
		static char tmp[1024];
		sprintf(tmp, "quakelib.Vec2(%g,%g)", (*$self)[0], (*$self)[1]);
		return tmp;
	}
};

%extend quakelib::Vec<3> {
	char *__str__(void) {
		static char tmp[1024];
		std::stringstream	ss;
		std::string			res;
		ss << (*$self);
		res = ss.str();
		res.resize(1023);
		sprintf(tmp, "%s", res.c_str());
		return tmp;
	}
	
	char *__repr__(void) {
		static char tmp[1024];
		sprintf(tmp, "quakelib.Vec3(%g,%g,%g)", (*$self)[0], (*$self)[1], (*$self)[2]);
		return tmp;
	}
};

%extend quakelib::OpCount {
	char *__str__(void) {
		static char tmp[1024];
		std::stringstream	ss;
		std::string			res;
		ss << (*$self);
		res = ss.str();
		res.resize(1023);
		sprintf(tmp, "%s", res.c_str());
		return tmp;
	}
	
	char *__repr__(void) {
		static char tmp[1024];
		sprintf(tmp, "<quakelib::OpCount at %p>", $self);
		return tmp;
	}
};

%extend quakelib::EQSimErrors {
	char *__str__(void) {
		static char tmp[32768];
		std::stringstream	ss;
		std::string			res;
		$self->write(ss);
		res = ss.str();
		res.resize(32767);
		sprintf(tmp, "%s", res.c_str());
		return tmp;
	}
	
	char *__repr__(void) {
		static char tmp[1024];
		sprintf(tmp, "<quakelib::EQSimErrors at %p>", $self);
		return tmp;
	}
};

%extend quakelib::Tensor<3,3> {
	char *__str__(void) {
		static char tmp[32768];
		std::stringstream	ss;
		std::string			res;
		ss << (*$self);
		res = ss.str();
		res.resize(32767);
		sprintf(tmp, "%s", res.c_str());
		return tmp;
	}
	
	char *__repr__(void) {
		static char tmp[1024];
		sprintf(tmp, "<quakelib::Tensor<3,3> at %p>", $self);
		return tmp;
	}
};

%extend quakelib::TensorRow<3> {
	char *__str__(void) {
		static char tmp[32768];
		std::stringstream	ss;
		std::string			res;
		ss << (*$self);
		res = ss.str();
		res.resize(32767);
		sprintf(tmp, "%s", res.c_str());
		return tmp;
	}
	
	char *__repr__(void) {
		static char tmp[1024];
		sprintf(tmp, "<quakelib::TensorRow<3> at %p>", $self);
		return tmp;
	}
};

%extend quakelib::RectBound<2> {
	char *__str__(void) {
		static char tmp[1024];
		std::stringstream	ss;
		std::string			res;
		ss << (*$self);
		res = ss.str();
		res.resize(1023);
		sprintf(tmp, "%s", res.c_str());
		return tmp;
	}
	
	char *__repr__(void) {
		static char tmp[1024];
		sprintf(tmp, "quakelib.RectBound<2> at %p", $self);
		return tmp;
	}
};

%extend quakelib::RectBound<3> {
	char *__str__(void) {
		static char tmp[1024];
		std::stringstream	ss;
		std::string			res;
		ss << (*$self);
		res = ss.str();
		res.resize(1023);
		sprintf(tmp, "%s", res.c_str());
		return tmp;
	}
	
	char *__repr__(void) {
		static char tmp[1024];
		sprintf(tmp, "quakelib.RectBound<3> at %p", $self);
		return tmp;
	}
};

%extend quakelib::Octree<2> {
	char *__repr__(void) {
		static char tmp[1024];
		sprintf(tmp, "quakelib.Octree<2> at %p", $self);
		return tmp;
	}
};

%extend quakelib::Octree<3> {
	char *__repr__(void) {
		static char tmp[1024];
		sprintf(tmp, "quakelib.Octree<3> at %p", $self);
		return tmp;
	}
};

// Map a Python sequence into any sized C double array
%typemap(in) double[ANY](double temp[$1_dim0]) {
  int i;
  if (!PySequence_Check($input)) {
	  PyErr_SetString(PyExc_TypeError,"Expecting a sequence");
	  return NULL;
  }
  if (PyObject_Length($input) != $1_dim0) {
	  PyErr_SetString(PyExc_ValueError,"Expecting a sequence with $1_dim0 elements");
	  return NULL;
  }
  for (i =0; i < $1_dim0; i++) {
	  PyObject *o = PySequence_GetItem($input,i);
	  if (!PyFloat_Check(o)) {
		 Py_XDECREF(o);
		 PyErr_SetString(PyExc_ValueError,"Expecting a sequence of floats");
		 return NULL;
	  }
	  temp[i] = PyFloat_AsDouble(o);
	  Py_DECREF(o);
  }
  $1 = &temp[0];
}

