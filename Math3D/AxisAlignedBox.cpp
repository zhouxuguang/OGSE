#include "AxisAlignedBox.h"
#include <assert.h>

NS_MATH3D_BEGIN

AxisAlignedBox::AxisAlignedBox(void)
{
	mMinimum = Vector3(1,1,1);
	mMaximum = Vector3(0,0,0);
}

AxisAlignedBox::AxisAlignedBox(const Vector3& min, const Vector3& max)
{
	SetMinimum(min);
	SetMaximum(max);
}

AxisAlignedBox& AxisAlignedBox::operator=(const AxisAlignedBox& rhs)
{
	if (rhs.IsNull())
		SetNull();
	else
		SetExtent(rhs.mMinimum, rhs.mMaximum);

	return *this;
}

AxisAlignedBox::~AxisAlignedBox(void)
{
}

void AxisAlignedBox::SetMinimum(const Vector3& vec)
{
	mMinimum = vec;
}

void AxisAlignedBox::SetMinimum(Real x, Real y, Real z)
{
	mMinimum.x = x;
	mMinimum.y = y;
	mMinimum.z = z;
}

void AxisAlignedBox::SetMinimumX(Real x)
{
	mMinimum.x = x;
}

void AxisAlignedBox::SetMinimumY(Real y)
{
	mMinimum.y = y;
}

void AxisAlignedBox::SetMinimumZ(Real z)
{
	mMinimum.z = z;
}

void AxisAlignedBox::SetMaximum(const Vector3& vec)
{
	mMaximum = vec;
}

void AxisAlignedBox::SetMaximum(Real x, Real y, Real z)
{
	mMaximum.x = x;
	mMaximum.y = y;
	mMaximum.z = z;
}

void AxisAlignedBox::SetMaximumX(Real x)
{
	mMaximum.x = x;
}

void AxisAlignedBox::SetMaximumY(Real y)
{
	mMaximum.y = y;
}

void AxisAlignedBox::SetMaximumZ(Real z)
{
	mMaximum.z = z;
}

void AxisAlignedBox::SetExtent(Real mx, Real my, Real mz, Real Mx, Real My, Real Mz)
{
	assert( (mx <= Mx && my <= My && mz <= Mz) &&
		"The minimum corner of the box must be less than or equal to maximum corner" );

	mMinimum.x = mx;
	mMinimum.y = my;
	mMinimum.z = mz;

	mMaximum.x = Mx;
	mMaximum.y = My;
	mMaximum.z = Mz;
}

void AxisAlignedBox::SetExtent(const Vector3& min, const Vector3& max)
{
	assert( (min.x <= max.x && min.y <= max.y && min.z <= max.z) &&
		"The minimum corner of the box must be less than or equal to maximum corner" );

	mMinimum = min;
	mMaximum = max;
}

const Vector3& AxisAlignedBox::GetMinimum(void) const
{
	return mMinimum;
}

Vector3& AxisAlignedBox::GetMinimum(void)
{
	return mMinimum;
}

const Vector3& AxisAlignedBox::GetMaximum(void) const
{
	return mMaximum;
}

Vector3& AxisAlignedBox::GetMaximum(void)
{
	return mMaximum;
}

bool AxisAlignedBox::IsNull() const
{
	return (mMinimum.x > mMaximum.x) || (mMinimum.y > mMaximum.y) || (mMinimum.z > mMaximum.z);
}

void AxisAlignedBox::SetNull()
{
	mMinimum.x = mMinimum.y = mMinimum.z = DBL_MAX;
	mMaximum.x = mMaximum.y = mMaximum.z = -DBL_MAX;
}

void AxisAlignedBox::Merge(const Vector3& vkPoint)
{
	if (vkPoint.x < mMinimum.x) mMinimum.x = vkPoint.x;
	if (vkPoint.x > mMaximum.x) mMaximum.x = vkPoint.x;
	if (vkPoint.y < mMinimum.x) mMinimum.y = vkPoint.y;
	if (vkPoint.y > mMaximum.x) mMaximum.y = vkPoint.y;
	if (vkPoint.z < mMinimum.x) mMinimum.z = vkPoint.z;
	if (vkPoint.z > mMaximum.x) mMaximum.z = vkPoint.z;
}

void AxisAlignedBox::Merge(const AxisAlignedBox& vkBox)
{
	if (vkBox.mMinimum.x < mMinimum.x) mMinimum.x = vkBox.mMinimum.x;
	if (vkBox.mMaximum.x > mMaximum.x) mMaximum.x = vkBox.mMaximum.x;

	if (vkBox.mMinimum.y < mMinimum.x) mMinimum.y = vkBox.mMinimum.y;
	if (vkBox.mMaximum.y > mMaximum.x) mMaximum.y = vkBox.mMaximum.y;

	if (vkBox.mMinimum.z < mMinimum.x) mMinimum.z = vkBox.mMinimum.z;
	if (vkBox.mMaximum.z > mMaximum.x) mMaximum.z = vkBox.mMaximum.z;
}

bool AxisAlignedBox::Contains(const Vector3 &vkPoint) const
{
	return vkPoint.x >= mMinimum.x && vkPoint.x <= mMaximum.x &&
		vkPoint.y >= mMinimum.y && vkPoint.y <= mMaximum.y &&
		vkPoint.z >= mMinimum.z && vkPoint.z <= mMaximum.z;
}

bool AxisAlignedBox::Contains(const AxisAlignedBox &vkBox) const
{
	if (vkBox.IsNull() || this->IsNull())
		return false;

	return this->mMinimum.x <= vkBox.mMinimum.x &&
		this->mMinimum.y <= vkBox.mMinimum.y &&
		this->mMinimum.z <= vkBox.mMinimum.z &&
		vkBox.mMaximum.x <= this->mMaximum.x &&
		vkBox.mMaximum.y <= this->mMaximum.y &&
		vkBox.mMaximum.z <= this->mMaximum.z;
}

bool AxisAlignedBox::Intersects(const AxisAlignedBox& b2) const
{
	if (this->IsNull() || b2.IsNull())
		return false;

	if (mMaximum.x < b2.mMinimum.x)
		return false;
	if (mMaximum.y < b2.mMinimum.y)
		return false;
	if (mMaximum.z < b2.mMinimum.z)
		return false;

	if (mMinimum.x > b2.mMaximum.x)
		return false;
	if (mMinimum.y > b2.mMaximum.y)
		return false;
	if (mMinimum.z > b2.mMaximum.z)
		return false;

	// otherwise, must be intersecting
	return true;
}

bool AxisAlignedBox::Intersects(const Plane3D& p) const
{
	return false;
	//return Math::intersects(p, *this);
}

Vector3 AxisAlignedBox::GetCenter(void) const
{
	return Vector3(
		(mMaximum.x + mMinimum.x) * 0.5f,
		(mMaximum.y + mMinimum.y) * 0.5f,
		(mMaximum.z + mMinimum.z) * 0.5f);
}

Vector3 AxisAlignedBox::GetCorner(Corner cornerToGet) const
{
	switch(cornerToGet)
	{
	case FAR_LEFT_BOTTOM:
		return mMinimum;
	case FAR_LEFT_TOP:
		return Vector3(mMinimum.x, mMaximum.y, mMinimum.z);
	case FAR_RIGHT_TOP:
		return Vector3(mMaximum.x, mMaximum.y, mMinimum.z);
	case FAR_RIGHT_BOTTOM:
		return Vector3(mMaximum.x, mMinimum.y, mMinimum.z);
	case NEAR_RIGHT_BOTTOM:
		return Vector3(mMaximum.x, mMinimum.y, mMaximum.z);
	case NEAR_LEFT_BOTTOM:
		return Vector3(mMinimum.x, mMinimum.y, mMaximum.z);
	case NEAR_LEFT_TOP:
		return Vector3(mMinimum.x, mMaximum.y, mMaximum.z);
	case NEAR_RIGHT_TOP:
		return mMaximum;
	default:
		return Vector3();
	}
}

NS_MATH3D_END
