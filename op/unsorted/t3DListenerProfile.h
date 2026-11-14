//	t3DListenerProfile.h: t3DListenerProfile, the listener's position, orientation and velocity.
// Evidence:  class; the SDK library's symbols name every member (tgsound.h in the original)
#ifndef t3DListenerProfile_h
#define t3DListenerProfile_h 1
#include "unsorted/tEntity.h"   // tPoint3, in its copy of the SDK's tvectormath.h
// The virtual functions in vtable order (0x00798f44), which is the order of the SDK library's
// vtable for the class; MSVC places overloads in reverse declaration order. The two by-value
// setters of each vector compile to the same bytes, so the exe holds one body for both slots.
// The double makes the class 8-aligned, so the compiler pads the vtable pointer to 8.
class t3DListenerProfile {
public:
    t3DListenerProfile();
    t3DListenerProfile(tPoint3 *a3DPosition, tPoint3 *aForwardDirection, tPoint3 *anUpDirection, tPoint3 *aVelocity, double aUnitScaleInMeters);
    t3DListenerProfile(tPoint3 a3DPosition, tPoint3 aForwardDirection, tPoint3 anUpDirection, tPoint3 aVelocity, double aUnitScaleInMeters);
    virtual ~t3DListenerProfile();
    virtual void mInit();
    virtual void mSet3DPosition(tPoint3 *aPoint);
    virtual void mSet3DPosition(tPoint3 aPoint);
    virtual void mSet3DPosition(float aX, float aY, float aZ);
    virtual tPoint3 *mGet3DPosition();
    virtual void mSetForwardDirection(tPoint3 *aPoint);
    virtual void mSetForwardDirection(tPoint3 aPoint);
    virtual void mSetForwardDirection(float aX, float aY, float aZ);
    virtual tPoint3 *mGetForwardDirection();
    virtual void mSetUpDirection(tPoint3 *aPoint);
    virtual void mSetUpDirection(tPoint3 aPoint);
    virtual void mSetUpDirection(float aX, float aY, float aZ);
    virtual tPoint3 *mGetUpDirection();
    virtual void mSetVelocity(tPoint3 *aPoint);
    virtual void mSetVelocity(tPoint3 aPoint);
    virtual void mSetVelocity(float aX, float aY, float aZ);
    virtual tPoint3 *mGetVelocity();
    virtual void mSetUnitScaleInMeters(double aScale);
    virtual double mGetUnitScaleInMeters();
    virtual void mDump();
    t3DListenerProfile& operator=(t3DListenerProfile& other);
    // HYPOTHESIS: the vectors' names follow mDump's labels and the f prefix of fUnitScaleInMeters.
    tPoint3 f3DPosition;
    tPoint3 fForwardDirection;
    tPoint3 fUpDirection;
    tPoint3 fVelocity;
    double fUnitScaleInMeters;
};
inline t3DListenerProfile::t3DListenerProfile()
{
    f3DPosition.x = 0.0f; f3DPosition.y = 0.0f; f3DPosition.z = 1.0f;
    fForwardDirection.x = 0.0f; fForwardDirection.y = 0.0f; fForwardDirection.z = -1.0f;
    fUpDirection.x = 0.0f; fUpDirection.y = 1.0f; fUpDirection.z = 0.0f;
    fVelocity.x = 0.0f; fVelocity.y = 0.0f; fVelocity.z = 0.0f;
    fUnitScaleInMeters = 1.0;
}
#endif
