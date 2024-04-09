
#include "Clipping.hpp"

namespace potato {
    unsigned int poopy(Vec4f v, float left,
                                                float right, float bottom,
                                                float top, float near,
                                                float far) {
        int reVal = 0x0;
        if ((left + v.x) < 0.0f) {
            reVal |= BIT_LEFT;
        }
        if ((right - v.x) < 0.0f) {
            reVal |= BIT_RIGHT;
        }
        if ((bottom + v.y) < 0.0f) {
            reVal |= BIT_BOTTOM;
        }
        if ((top - v.y) < 0.0f) {
            reVal |= BIT_TOP;
        }
        if ((near + v.z) < 0.0f) {
            reVal |= BIT_NEAR;
        }
        if ((far - v.z) < 0.0f) {
            reVal |= BIT_FAR;
        }

        return reVal;
    }

    unsigned int getExtendedCohenSutherlandCode(Vec4f v, float left,
                                                float right, float bottom,
                                                float top, float near,
                                                float far) {
        int reVal = 0x0;
        if (left > v.x) {
            reVal |= BIT_LEFT;
        }
        if (right < v.x) {
            reVal |= BIT_RIGHT;
        }
        if (bottom > v.y) {
            reVal |= BIT_BOTTOM;
        }
        if (top < v.y) {
            reVal |= BIT_TOP;
        }
        if (near > v.z) {
            reVal |= BIT_NEAR;
        }
        if (far < v.z) {
            reVal |= BIT_FAR;
        }

        return reVal;
    }

    inline unsigned int getExtendedCohenSutherlandCode(Vec4f v) {
        return getExtendedCohenSutherlandCode(v, CLIP_LEFT, CLIP_RIGHT,
                                              CLIP_BOTTOM, CLIP_TOP, CLIP_NEAR,
                                              CLIP_FAR);
    };
}; // namespace potato
