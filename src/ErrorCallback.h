//
// Created by Darrow, Jackson on 11/13/17.
//

#ifndef INC_3DBILLIARDS_ERRORCALLBACK_H
#define INC_3DBILLIARDS_ERRORCALLBACK_H

#include "foundation/PxErrorCallback.h"
#include "PxPhysXConfig.h"

class ErrorCallback : public PxErrorCallback {
public:
	void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) {
        std::cerr << code << " " << message;
	}
};


#endif //INC_3DBILLIARDS_ERRORCALLBACK_H
