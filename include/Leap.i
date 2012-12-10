################################################################################
# Copyright (C) 2012 Leap Motion, Inc. All rights reserved.                    #
# NOTICE: This developer release of Leap Motion, Inc. software is confidential #
# and intended for very limited distribution. Parties using this software must #
# accept the SDK Agreement prior to obtaining this software and related tools. #
# This software is subject to copyright.                                       #
################################################################################

# usage:
#   swig -c++ -python -o Leap_wrap_python.cpp -interface _LeapPython Leap.i
#   swig -c++ -java   -o Leap_wrap_java.cpp -package com.leapmotion.leap -outdir com/leapmotion/leap Leap.i
#   swig -c++ -csharp -o Leap_wrap_csharp.cpp -dllimport _LeapCSharp -namespace Leap Leap.i

%module(directors="1", threads="1") Leap
#pragma SWIG nowarn=325,362

%include "std_string.i"
%include "std_vector.i"
%include "stdint.i"

%ignore Leap::Finger::Finger();
%ignore Leap::Finger::Finger(const FingerImplementation&);
%ignore Leap::Hand::Hand();
%ignore Leap::Hand::Hand(const HandImplementation&);
%ignore Leap::Frame::Frame();
%ignore Leap::Frame::Frame(const FrameImplementation&);
%ignore Leap::Controller::Controller(const ControllerImplementation&);

#if SWIGPYTHON
%begin %{
#if defined(_WIN32) && defined(_DEBUG)
// Workaround for obscure STL template error
#include <vector>
// Workaround for non-existent Python debug library
#define _TMP_DEBUG _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG _TMP_DEBUG
#undef _TMP_DEBUG
#endif
%}
#endif

#if SWIGCSHARP
%begin %{
#if defined(_WIN32)
#include <windows.h>
// When dynamically loading the Leap C# DLL, set the DLL search path to look in
// the same the directory. This will allow loading the Leap.dll. Create the
// Leap C# DLL with the /DELAYLOAD:Leap.dll link option.
extern "C" BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD fdwReason,
    _In_ LPVOID lpvReserved)
{
  if (lpvReserved == 0) {
    static TCHAR lpPrevPathName[1024];
    static BOOL restore = FALSE;

    if (fdwReason == DLL_PROCESS_ATTACH) {
      TCHAR lpPathName[1024];
      int len;

      len = GetDllDirectory(static_cast<DWORD>(sizeof(lpPrevPathName) - 1),
                            lpPrevPathName);
      if (len < 0 && len >= sizeof(lpPrevPathName)) {
        len = 0;
      }
      lpPrevPathName[len] = '\0';
      len = static_cast<int>(GetModuleFileName(static_cast<HMODULE>(hinstDLL),
                         lpPathName, static_cast<DWORD>(sizeof(lpPathName))));
      if (len > 0 && len < sizeof(lpPathName)) {
        for (int i = len; i >= 0; i--) {
          if (lpPathName[i] == '\\' || lpPathName[i] == '/') {
            lpPathName[i] = '\0';
            restore = SetDllDirectory(lpPathName);
            break;
          }
        }
      }
    } else if (fdwReason == DLL_PROCESS_DETACH) {
      if (restore && lpPrevPathName[0] != '\0') {
        SetDllDirectory(lpPrevPathName);
        restore = FALSE;
      }
    }
  }
  return TRUE;
}
#endif
%}
#endif

%header %{
#define SWIG
#include "Leap.h"
%}

%template(FingerArray) std::vector<Leap::Finger>;
%template(HandArray) std::vector<Leap::Hand>;

%feature("director") Leap::Listener;
#if SWIGPYTHON
%feature("director:except") {
  if ($error != NULL) {
    PyErr_Print();
  }
}
#endif

%pragma(java) jniclasscode=%{
  static {
    try {
      System.loadLibrary("_LeapJava");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. \n" + e);
      System.exit(1);
    }
  }
%}

%include "Leap.h"
