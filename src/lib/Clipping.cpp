
#include "Clipping.hpp"

namespace potato {
    unsigned int getExtendedCohenSutherlandCode(Vec4f v, float left,
                                                float right, float bottom,
                                                float top, float near,
                                                float far) {
        int reVal = 0x0;
    if((-v.x + left * v.w) > 0.0f){
            reVal |= BIT_LEFT;
        }

    if((v.x - right * v.w) > 0.0f){
            reVal |= BIT_RIGHT;
        }
    if((-v.y + bottom * v.w) > 0.0f){
            reVal |= BIT_BOTTOM;
        }
    if((v.y - top * v.w) > 0.0f){
            reVal |= BIT_TOP;
        }
    if((-v.z + near * v.w) > 0.0f){
            reVal |= BIT_NEAR;
        }
    if((v.z - far * v.w) > 0.0f){
            reVal |= BIT_FAR;
        }

        return reVal;
    }

}; // namespace potato
