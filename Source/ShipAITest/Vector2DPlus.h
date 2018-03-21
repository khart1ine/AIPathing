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
	FVector2DPlus() : FVector2D() {}
	FVector2DPlus(float InX, float InY) : FVector2D(InX, InY) {}

	/** Use copy constructor of Parent **/
	FVector2DPlus(const FVector2D& other) : FVector2D(other){}

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

	//----------------------- GetReverse ----------------------------------------
	//
	//  returns the vector that is the reverse of this vector
	//------------------------------------------------------------------------
	static inline FVector2DPlus Reverse(FVector2DPlus VecIn)
	{
		return FVector2DPlus(-VecIn.X, -VecIn.Y);
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
		FVector2D::operator *=(Rhs);
		return *this;
	}

	const FVector2DPlus& operator+=(const FVector2DPlus &Rhs)
	{
		FVector2D::operator +=(Rhs);
		return *this;
	}


	//------------------------------ Perp ------------------------------------
	//
	//  Returns a vector perpendicular to this vector
	//------------------------------------------------------------------------
	static inline FVector2DPlus  Perp(FVector2DPlus VecIn)
	{
		return FVector2DPlus(-VecIn.Y, VecIn.X);
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

inline float Vec2DDistance(const FVector2DPlus &v1, const FVector2DPlus &v2);
//-------------------- LinesIntersection2D-------------------------
//
//	Given 2 lines in 2D space AB, CD this returns true if an 
//	intersection occurs and sets dist to the distance the intersection
//  occurs along AB. Also sets the 2d vector point to the point of
//  intersection
//----------------------------------------------------------------- 
inline bool LineIntersection2D(FVector2DPlus   A,
	FVector2DPlus  B,
	FVector2DPlus   C,
	FVector2DPlus   D,
	float&     dist,
	FVector2DPlus&  point)
{

	float rTop = (A.Y - C.Y)*(D.X - C.X) - (A.X - C.X)*(D.Y - C.Y);
	float rBot = (B.X - A.X)*(D.Y - C.Y) - (B.Y - A.Y)*(D.X - C.X);

	float sTop = (A.Y - C.Y)*(B.X - A.X) - (A.X - C.X)*(B.Y - A.Y);
	float sBot = (B.X - A.X)*(D.Y - C.Y) - (B.Y - A.Y)*(D.X - C.X);

	if ((rBot == 0) || (sBot == 0))
	{
		//lines are parallel
		return false;
	}

	float r = rTop / rBot;
	float s = sTop / sBot;

	if ((r > 0) && (r < 1) && (s > 0) && (s < 1))
	{
		dist = Vec2DDistance(A, B) * r;

		point = A + (r * (B - A));

		return true;
	}

	else
	{
		dist = 0;

		return false;
	}
}

inline float Vec2DDistance(const FVector2DPlus &v1, const FVector2DPlus &v2)
{

	float ySeparation = v2.Y - v1.Y;
	float xSeparation = v2.X - v1.X;

	return sqrt(ySeparation*ySeparation + xSeparation * xSeparation);
}