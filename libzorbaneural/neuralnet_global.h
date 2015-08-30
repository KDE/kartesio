#ifndef NEURALNET_GLOBAL_H
#define NEURALNET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined ZNNLIB
#define ZORBANN_DLLSPEC  Q_DECL_EXPORT
#else
#define ZORBANN_DLLSPEC Q_DECL_IMPORT
#endif

#endif // NEURALNET_GLOBAL_H
