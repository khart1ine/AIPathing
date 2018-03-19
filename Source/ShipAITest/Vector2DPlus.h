// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Math/Vector2D.h"
#include "Vector2DPlus.generated.h"

/**
*
*	Name:  Vector2DPlus.h
*
*  Desc:   Extended version of Unreal's Vector2D with additional 2D functionality
*
*  Author: Marc Aubanel for extension based on Programming Game AI By Example
*

*/

USTRUCT(BlueprintType)
struct FVector2DPlus : public FVector2D 
{
	GENERATED_BODY()

public:
	FVector2DPlus(){X = 0.0f; Y = 0.0f;}
	FVector2DPlus(float A, float B){X=A; Y=B;}

	/*........................Truncate.........................................

	Truncates a vector so it doesn't pass the max

	..................................................................*/
	inline void Truncate(float Max)
	{
		if (this->Size() > Max)
		{
			this->Normalize();

			*this *= Max;
		}
	}

	/*........................Zero.........................................

	Zeros out struct

	..................................................................*/
	void Zero() {X = 0.0f; Y = 0.0f; }

	/*........................GetNormal.........................................

	Returns the normal of this vector without changing it

	..................................................................*/
	inline FVector2DPlus GetNormal()
	{

		FVector2DPlus Normalized(0.0f, 0.0f);
		float VectorLength = this->Size();

		if (VectorLength > 0)
		{
			Normalized.X = X / VectorLength;
			Normalized.Y = Y / VectorLength;
		}

		return Normalized;
	}

	/*........................GetVectorFromAngleInDegrees.........................................

	Returns a normalized vector when passed in an angle in degrees

	..................................................................*/
	static inline FVector2DPlus GetVectorFromAngleInDegrees(float Angle)
	{
		Angle = FMath::DegreesToRadians(Angle);
		return (FVector2DPlus(FMath::Cos(Angle), FMath::Sin(Angle)));
	}


	/** Operators overloaded **/
	const FVector2DPlus& operator *=(const float& Rhs)
	{
		X *= Rhs;
		Y *= Rhs;

		return *this;
	}

	const FVector2DPlus& operator+=(const FVector2DPlus &rhs)
	{
		X += rhs.X;
		Y += rhs.Y;

		return *this;
	}

	//------------------------------ Perp ------------------------------------
	//
	//  Returns a vector perpendicular to this vector
	//------------------------------------------------------------------------
	inline FVector2DPlus  Perp()const
	{
		return FVector2DPlus(-Y, X);
	}



	/*........................SphericalToVector.........................................

	Returns a  vector when passed in an anglein degrees and a magnitude

	..................................................................*/
	static inline FVector2DPlus SphericalToVector(float Angle, float Magnitude)
	{
		float X = Magnitude * FMath::Cos(FMath::DegreesToRadians(Angle));
		float Y = Magnitude * FMath::Sin(FMath::DegreesToRadians(Angle));
		return FVector2DPlus(X, Y);
	}

	static inline float Diagonal(float Width, float Height, float Adjustment = 0)
	{
		if (Adjustment)
		{
			Height += Adjustment;
			Width += Adjustment;
		}
		return (FMath::Sqrt(FMath::Square(Width) + FMath::Square(Height)));
	}

};


/** Operators overloaded as free functions so that these can be used with a non calling Vector2DPlus**/

inline FVector2DPlus operator-(const FVector2DPlus &Lhs, const FVector2DPlus &Rhs)
{
	FVector2DPlus result(Lhs);

	result.X -= Rhs.X;
	result.Y -= Rhs.Y;

	return result;
}

inline FVector2DPlus operator+(const FVector2DPlus &Lhs, const FVector2DPlus &Rhs)
{
	FVector2DPlus result(Lhs);

	result.X += Rhs.X;
	result.Y += Rhs.Y;

	return result;
}

inline FVector2DPlus operator*(const FVector2DPlus &Lhs, float Rhs)
{
	FVector2DPlus result(Lhs);
	result *= Rhs;
	return result;
}

inline FVector2DPlus operator*(const FVector2DPlus &Lhs, const FVector2DPlus &Rhs)
{
	FVector2DPlus result(Lhs);
	result.X *= Rhs.X;
	result.Y *= Rhs.Y;
	return result;
}

inline FVector2DPlus operator/(const FVector2DPlus &Lhs, float val)
{
	FVector2DPlus result(Lhs);
	result.X /= val;
	result.Y /= val;

	return result;
}



