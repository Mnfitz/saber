/////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2024 Matthew Fitzgerald
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
/////////////////////////////////////////////////////////////////////

// catch2
#include <catch2/benchmark/catch_benchmark.hpp>
#include "catch2/catch_template_test_macros.hpp"

// saber
#include "saber/geometry/point.hpp"
#include "saber/geometry/size.hpp"
#include "saber/geometry/rectangle.hpp"
#include "saber/geometry/matrix.hpp"

// std
#include <array>
#include <ctime>
#include <iostream>
#include <string>

template<typename T>
constexpr const char* TypeName()
{
	if constexpr (std::is_same_v<T, int>) return "int";
	else if constexpr (std::is_same_v<T, float>) return "float";
	else if constexpr (std::is_same_v<T, double>) return "double";
	else return "unknown";
}

template<saber::geometry::ImplKind Impl>
constexpr const char* ImplName()
{
	if constexpr (Impl == saber::geometry::ImplKind::kScalar) return "kScalar";
	else if constexpr (Impl == saber::geometry::ImplKind::kSimd) return "kSimd";
	else return "unknown";
}

template<typename T, saber::geometry::ImplKind Impl>
std::string WorkloadName(const char* inWorkload)
{
	return std::string(inWorkload) + "<" + TypeName<T>() + ", " + ImplName<Impl>() + "> ";
}

int GauranteedNotConstexpr()
{
	// TRICKY j3fitz 30nov2024: Prevent compiler from optimizing away all benchmark work.
	// By using a value in the benchmark computation that can't be known at
	// compile-time, we force the compiler to emit code to perform computation at
	// runtime. Otherwise, the compiler is smart enuf to do the math at compile-time
	// and just emit a final answer at runtime... defeating the purpose of the benchmark!

	// Get current time: "unknowable at compile-time"
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);
	const auto dayOfWeek = now->tm_wday;
	auto gauranteedNotConstexpr = dayOfWeek + 1; // +1: -> [1...7]
	return gauranteedNotConstexpr;
}

template<typename T, saber::geometry::ImplKind Impl>
saber::geometry::Point<T, Impl> sPoint{};

template<typename T, saber::geometry::ImplKind Impl>
void PointAddWork()
{
	const auto point1 = sPoint<T, Impl>;
	const auto point2 = saber::geometry::Point<T, Impl>{ 2, -3 };
	auto point = point1 + point2;

	point += point + point1;
	point += point + point2;
	point += point + point1;
	point += point + point2;
	point += point + point1;
	point += point + point2;
	point += point + point1;
	point += point + point2;
	sPoint<T, Impl> += point;
}

template<typename T, saber::geometry::ImplKind Impl>
void PointSubWork()
{
	const auto point1 = sPoint<T, Impl>;
	const auto point2 = saber::geometry::Point<T, Impl>{ 2, -3 };
	auto point = point1 - point2;

	point -= point - point1;
	point -= point - point2;
	point -= point - point1;
	point -= point - point2;
	point -= point - point1;
	point -= point - point2;
	point -= point - point1;
	point -= point - point2;
	sPoint<T, Impl> -= point;
}

template<typename T, saber::geometry::ImplKind Impl>
void PointMulWork()
{
	const auto point1 = sPoint<T, Impl>;
	const auto point2 = saber::geometry::Point<T, Impl>{ 2, -3 };
	auto point = point1 * point2;

	point *= point * point1;
	point *= point * point2;
	point *= point * point1;
	point *= point * point2;
	point *= point * point1;
	point *= point * point2;
	point *= point * point1;
	point *= point * point2;
	sPoint<T, Impl> *= point;
}

template<typename T, saber::geometry::ImplKind Impl>
void PointDivWork()
{
	const auto point1 = sPoint<T, Impl>;
	const auto point2 = saber::geometry::Point<T, Impl>{ -1, -1 };
	auto point = point1 / point2;

	point /= point / point1;
	point /= point / point2;
	point /= point / point1;
	point /= point / point2;
	point /= point / point1;
	point /= point / point2;
	point /= point / point1;
	point /= point / point2;
	sPoint<T, Impl> /= point;
}

template<typename T, saber::geometry::ImplKind Impl>
void PointScaleWork()
{
	const auto point1 = sPoint<T, Impl>;
	const auto point2 = saber::geometry::Point<T, Impl>{ -2, 3 };
	auto point = saber::geometry::Scale(point1, point2);

	point.Scale(saber::geometry::Scale(point, point1));
	point.Scale(saber::geometry::Scale(point, point2));
	point.Scale(saber::geometry::Scale(point, point1));
	point.Scale(saber::geometry::Scale(point, point2));
	point.Scale(saber::geometry::Scale(point, point1));
	point.Scale(saber::geometry::Scale(point, point2));
	point.Scale(saber::geometry::Scale(point, point1));
	point.Scale(saber::geometry::Scale(point, point2));
	sPoint<T, Impl>.Scale(point);
}

template<typename T, saber::geometry::ImplKind Impl>
void PointTranslateWork()
{
	const auto point1 = sPoint<T, Impl>;
	const auto point2 = saber::geometry::Point<T, Impl>{ -2, 3 };
	auto point = saber::geometry::Translate(point1, point2);

	point.Translate(saber::geometry::Translate(point, point1));
	point.Translate(saber::geometry::Translate(point, point2));
	point.Translate(saber::geometry::Translate(point, point1));
	point.Translate(saber::geometry::Translate(point, point2));
	point.Translate(saber::geometry::Translate(point, point1));
	point.Translate(saber::geometry::Translate(point, point2));
	point.Translate(saber::geometry::Translate(point, point1));
	point.Translate(saber::geometry::Translate(point, point2));
	sPoint<T, Impl>.Translate(point);
}

template<typename T, saber::geometry::ImplKind Impl>
void PointCeilWork()
{
	auto point = saber::geometry::RoundCeil(sPoint<T, Impl>);

	point = saber::geometry::RoundCeil(point.RoundCeil());
	point = saber::geometry::RoundCeil(point.RoundCeil());
	point = saber::geometry::RoundCeil(point.RoundCeil());
	point = saber::geometry::RoundCeil(point.RoundCeil());
	point = saber::geometry::RoundCeil(point.RoundCeil());
	point = saber::geometry::RoundCeil(point.RoundCeil());
	point = saber::geometry::RoundCeil(point.RoundCeil());
	point = saber::geometry::RoundCeil(point.RoundCeil());
	sPoint<T, Impl> = saber::geometry::RoundCeil(point);
}

template<typename T, saber::geometry::ImplKind Impl>
void PointFloorWork()
{
	auto point = saber::geometry::RoundFloor(sPoint<T, Impl>);

	point = saber::geometry::RoundFloor(point.RoundFloor());
	point = saber::geometry::RoundFloor(point.RoundFloor());
	point = saber::geometry::RoundFloor(point.RoundFloor());
	point = saber::geometry::RoundFloor(point.RoundFloor());
	point = saber::geometry::RoundFloor(point.RoundFloor());
	point = saber::geometry::RoundFloor(point.RoundFloor());
	point = saber::geometry::RoundFloor(point.RoundFloor());
	point = saber::geometry::RoundFloor(point.RoundFloor());
	sPoint<T, Impl> = saber::geometry::RoundFloor(point);
}

template<typename T, saber::geometry::ImplKind Impl>
void PointNearestWork()
{
	auto point = saber::geometry::RoundNearest(sPoint<T, Impl>);

	point = saber::geometry::RoundNearest(point.RoundNearest());
	point = saber::geometry::RoundNearest(point.RoundNearest());
	point = saber::geometry::RoundNearest(point.RoundNearest());
	point = saber::geometry::RoundNearest(point.RoundNearest());
	point = saber::geometry::RoundNearest(point.RoundNearest());
	point = saber::geometry::RoundNearest(point.RoundNearest());
	point = saber::geometry::RoundNearest(point.RoundNearest());
	point = saber::geometry::RoundNearest(point.RoundNearest());
	sPoint<T, Impl> = saber::geometry::RoundNearest(point);
}

template<typename T, saber::geometry::ImplKind Impl>
void PointTruncWork()
{
	auto point = saber::geometry::RoundTrunc(sPoint<T, Impl>);

	point = saber::geometry::RoundTrunc(point.RoundTrunc());
	point = saber::geometry::RoundTrunc(point.RoundTrunc());
	point = saber::geometry::RoundTrunc(point.RoundTrunc());
	point = saber::geometry::RoundTrunc(point.RoundTrunc());
	point = saber::geometry::RoundTrunc(point.RoundTrunc());
	point = saber::geometry::RoundTrunc(point.RoundTrunc());
	point = saber::geometry::RoundTrunc(point.RoundTrunc());
	point = saber::geometry::RoundTrunc(point.RoundTrunc());
	sPoint<T, Impl> = saber::geometry::RoundTrunc(point);
}

TEMPLATE_TEST_CASE("saber::geometry::Point", "[saber][benchmark][template]", int, float, double)
{
	using namespace saber::geometry;

	const auto x = static_cast<TestType>(GauranteedNotConstexpr() + 0);
	const auto y = static_cast<TestType>(GauranteedNotConstexpr() + 1);

	sPoint<TestType, ImplKind::kScalar> = { x, y };
	sPoint<TestType, ImplKind::kSimd> = { x, y };

	const auto pointScalarName = WorkloadName<TestType, ImplKind::kScalar>("Point");
	const auto pointSimdName = WorkloadName<TestType, ImplKind::kSimd>("Point");

	BENCHMARK(pointScalarName + "operator+") { PointAddWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(pointSimdName + "operator+") { PointAddWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(pointScalarName + "operator-") { PointSubWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(pointSimdName + "operator-") { PointSubWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(pointScalarName + "operator*") { PointMulWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(pointSimdName + "operator*") { PointMulWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(pointScalarName + "operator/") { PointDivWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(pointSimdName + "operator/") { PointDivWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(pointScalarName + "Scale()") { PointScaleWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(pointSimdName + "Scale()") { PointScaleWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(pointScalarName + "Translate()") { PointTranslateWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(pointSimdName + "Translate()") { PointTranslateWork<TestType, ImplKind::kSimd>(); };

	if constexpr (std::is_floating_point_v<TestType>)
	{
		// Rounding APIs are only available for floating point types
		BENCHMARK(pointScalarName + "RoundCeil()") { PointCeilWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(pointSimdName + "RoundCeil()") { PointCeilWork<TestType, ImplKind::kSimd>(); };

		BENCHMARK(pointScalarName + "RoundFloor()") { PointFloorWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(pointSimdName + "RoundFloor()") { PointFloorWork<TestType, ImplKind::kSimd>(); };

		BENCHMARK(pointScalarName + "RoundNearest()") { PointNearestWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(pointSimdName + "RoundNearest()") { PointNearestWork<TestType, ImplKind::kSimd>(); };

		BENCHMARK(pointScalarName + "RoundTrunc()") { PointTruncWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(pointSimdName + "RoundTrunc()") { PointTruncWork<TestType, ImplKind::kSimd>(); };
	}
};

template<typename T, saber::geometry::ImplKind Impl>
saber::geometry::Size<T, Impl> sSize{};

template<typename T, saber::geometry::ImplKind Impl>
void SizeAddWork()
{
	const auto size1 = sSize<T, Impl>;
	const auto size2 = saber::geometry::Size<T, Impl>{ 2, -3 };

	auto size = size1 + size2;
	size += size + size1;
	size += size + size2;
	size += size + size1;
	size += size + size2;
	size += size + size1;
	size += size + size2;
	size += size + size1;
	size += size + size2;
	sSize<T, Impl> += size;
}

template<typename T, saber::geometry::ImplKind Impl>
void SizeSubWork()
{
	const auto size1 = sSize<T, Impl>;
	const auto size2 = saber::geometry::Size<T, Impl>{ 2, -3 };

	auto size = size1 - size2;
	size -= size - size1;
	size -= size - size2;
	size -= size - size1;
	size -= size - size2;
	size -= size - size1;
	size -= size - size2;
	size -= size - size1;
	size -= size - size2;
	sSize<T, Impl> -= size;
}

template<typename T, saber::geometry::ImplKind Impl>
void SizeMulWork()
{
	const auto size1 = sSize<T, Impl>;
	const auto size2 = saber::geometry::Size<T, Impl>{ 2, -3 };

	auto size = size1 * size2;
	size *= size * size1;
	size *= size * size2;
	size *= size * size1;
	size *= size * size2;
	size *= size * size1;
	size *= size * size2;
	size *= size * size1;
	size *= size * size2;
	sSize<T, Impl> *= size;
}

template<typename T, saber::geometry::ImplKind Impl>
void SizeDivWork()
{
	const auto size1 = sSize<T, Impl>;
	const auto size2 = saber::geometry::Size<T, Impl>{ -1, -1 };

	auto size = size1 / size2;
	size /= size / size1;
	size /= size / size2;
	size /= size / size1;
	size /= size / size2;
	size /= size / size1;
	size /= size / size2;
	size /= size / size1;
	size /= size / size2;
	sSize<T, Impl> /= size;
}

template<typename T, saber::geometry::ImplKind Impl>
void SizeScaleWork()
{
	const auto size1 = sSize<T, Impl>;
	const auto size2 = saber::geometry::Size<T, Impl>{ 2, -3 };
	auto size = Scale(size1, size2);

	size.Scale(saber::geometry::Scale(size, size1));
	size.Scale(saber::geometry::Scale(size, size2));
	size.Scale(saber::geometry::Scale(size, size1));
	size.Scale(saber::geometry::Scale(size, size2));
	size.Scale(saber::geometry::Scale(size, size1));
	size.Scale(saber::geometry::Scale(size, size2));
	size.Scale(saber::geometry::Scale(size, size1));
	size.Scale(saber::geometry::Scale(size, size2));
	sSize<T, Impl>.Scale(size);
}

template<typename T, saber::geometry::ImplKind Impl>
void SizeEnlargeWork()
{
	const auto size1 = sSize<T, Impl>;
	const auto size2 = saber::geometry::Size<T, Impl>{ 2, -3 };
	auto size = saber::geometry::Enlarge(size1, size2);

	size.Enlarge(saber::geometry::Enlarge(size, size1));
	size.Enlarge(saber::geometry::Enlarge(size, size2));
	size.Enlarge(saber::geometry::Enlarge(size, size1));
	size.Enlarge(saber::geometry::Enlarge(size, size2));
	size.Enlarge(saber::geometry::Enlarge(size, size1));
	size.Enlarge(saber::geometry::Enlarge(size, size2));
	size.Enlarge(saber::geometry::Enlarge(size, size1));
	size.Enlarge(saber::geometry::Enlarge(size, size2));
	sSize<T, Impl>.Enlarge(size);
}

template<typename T, saber::geometry::ImplKind Impl>
void SizeCeilWork()
{
	auto size = saber::geometry::RoundCeil(sSize<T, Impl>);

	size = saber::geometry::RoundCeil(size.RoundCeil());
	size = saber::geometry::RoundCeil(size.RoundCeil());
	size = saber::geometry::RoundCeil(size.RoundCeil());
	size = saber::geometry::RoundCeil(size.RoundCeil());
	size = saber::geometry::RoundCeil(size.RoundCeil());
	size = saber::geometry::RoundCeil(size.RoundCeil());
	size = saber::geometry::RoundCeil(size.RoundCeil());
	size = saber::geometry::RoundCeil(size.RoundCeil());
	sSize<T, Impl> = saber::geometry::RoundCeil(size);
}

template<typename T, saber::geometry::ImplKind Impl>
void SizeFloorWork()
{
	auto size = saber::geometry::RoundFloor(sSize<T, Impl>);

	size = saber::geometry::RoundFloor(size.RoundFloor());
	size = saber::geometry::RoundFloor(size.RoundFloor());
	size = saber::geometry::RoundFloor(size.RoundFloor());
	size = saber::geometry::RoundFloor(size.RoundFloor());
	size = saber::geometry::RoundFloor(size.RoundFloor());
	size = saber::geometry::RoundFloor(size.RoundFloor());
	size = saber::geometry::RoundFloor(size.RoundFloor());
	size = saber::geometry::RoundFloor(size.RoundFloor());
	sSize<T, Impl> = saber::geometry::RoundFloor(size);
}

template<typename T, saber::geometry::ImplKind Impl>
void SizeNearestWork()
{
	auto size = saber::geometry::RoundNearest(sSize<T, Impl>);

	size = saber::geometry::RoundNearest(size.RoundNearest());
	size = saber::geometry::RoundNearest(size.RoundNearest());
	size = saber::geometry::RoundNearest(size.RoundNearest());
	size = saber::geometry::RoundNearest(size.RoundNearest());
	size = saber::geometry::RoundNearest(size.RoundNearest());
	size = saber::geometry::RoundNearest(size.RoundNearest());
	size = saber::geometry::RoundNearest(size.RoundNearest());
	size = saber::geometry::RoundNearest(size.RoundNearest());
	sSize<T, Impl> = saber::geometry::RoundNearest(size);
}

template<typename T, saber::geometry::ImplKind Impl>
void SizeTruncWork()
{
	auto size = saber::geometry::RoundTrunc(sSize<T, Impl>);

	size = saber::geometry::RoundTrunc(size.RoundTrunc());
	size = saber::geometry::RoundTrunc(size.RoundTrunc());
	size = saber::geometry::RoundTrunc(size.RoundTrunc());
	size = saber::geometry::RoundTrunc(size.RoundTrunc());
	size = saber::geometry::RoundTrunc(size.RoundTrunc());
	size = saber::geometry::RoundTrunc(size.RoundTrunc());
	size = saber::geometry::RoundTrunc(size.RoundTrunc());
	size = saber::geometry::RoundTrunc(size.RoundTrunc());
	sSize<T, Impl> = saber::geometry::RoundTrunc(size);
}

TEMPLATE_TEST_CASE("saber::geometry::Size", "[saber][benchmark][template]", int, float, double)
{
	using namespace saber::geometry;

	const auto width = static_cast<TestType>(GauranteedNotConstexpr() + 0);
	const auto height = static_cast<TestType>(GauranteedNotConstexpr() + 1);

	sSize<TestType, ImplKind::kScalar> = { width, height };
	sSize<TestType, ImplKind::kSimd> = { width, height };

	const auto sizeScalarName = WorkloadName<TestType, ImplKind::kScalar>("Size");
	const auto sizeSimdName = WorkloadName<TestType, ImplKind::kSimd>("Size");

	BENCHMARK(sizeScalarName + "operator+") { SizeAddWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(sizeSimdName + "operator+") { SizeAddWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(sizeScalarName + "operator-") { SizeSubWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(sizeSimdName + "operator-") { SizeSubWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(sizeScalarName + "operator*") { SizeMulWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(sizeSimdName + "operator*") { SizeMulWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(sizeScalarName + "operator/") { SizeDivWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(sizeSimdName + "operator/") { SizeDivWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(sizeScalarName + "Scale()") { SizeScaleWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(sizeSimdName + "Scale()") { SizeScaleWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(sizeScalarName + "Enlarge()") { SizeEnlargeWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(sizeSimdName + "Enlarge()") { SizeEnlargeWork<TestType, ImplKind::kSimd>(); };

	if constexpr (std::is_floating_point_v<TestType>)
	{
		// Rounding APIs are only available for floating point types
		BENCHMARK(sizeScalarName + "RoundCeil()") { SizeCeilWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(sizeSimdName + "RoundCeil()") { SizeCeilWork<TestType, ImplKind::kSimd>(); };

		BENCHMARK(sizeScalarName + "RoundFloor()") { SizeFloorWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(sizeSimdName + "RoundFloor()") { SizeFloorWork<TestType, ImplKind::kSimd>(); };

		BENCHMARK(sizeScalarName + "RoundNearest()") { SizeNearestWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(sizeSimdName + "RoundNearest()") { SizeNearestWork<TestType, ImplKind::kSimd>(); };

		BENCHMARK(sizeScalarName + "RoundTrunc()") { SizeTruncWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(sizeSimdName + "RoundTrunc()") { SizeTruncWork<TestType, ImplKind::kSimd>(); };
	}
};

template<typename T, saber::geometry::ImplKind Impl>
saber::geometry::Rectangle<T, Impl> sRectangle{};

template<typename T, saber::geometry::ImplKind Impl>
void RectangleTranslateWork()
{
	const auto rect1 = sRectangle<T, Impl>;
	const auto point = saber::geometry::Point<T, Impl>{ 2, -3 };
	auto rect = Translate(rect1, point);

	rect.Translate(Translate(rect, point).Origin());
	rect.Translate(Translate(rect, point).Origin());
	rect.Translate(Translate(rect, point).Origin());
	rect.Translate(Translate(rect, point).Origin());
	rect.Translate(Translate(rect, point).Origin());
	rect.Translate(Translate(rect, point).Origin());
	rect.Translate(Translate(rect, point).Origin());
	rect.Translate(Translate(rect, point).Origin());
	sRectangle<T, Impl>.Translate(rect.Origin());
}

template<typename T, saber::geometry::ImplKind Impl>
void RectangleEnlargeWork()
{
	const auto rect1 = sRectangle<T, Impl>;
	const auto size = saber::geometry::Size<T, Impl>{ 2, -3 };
	auto rect = Enlarge(rect1, size);

	rect.Enlarge(Enlarge(rect, size).Size());
	rect.Enlarge(Enlarge(rect, size).Size());
	rect.Enlarge(Enlarge(rect, size).Size());
	rect.Enlarge(Enlarge(rect, size).Size());
	rect.Enlarge(Enlarge(rect, size).Size());
	rect.Enlarge(Enlarge(rect, size).Size());
	rect.Enlarge(Enlarge(rect, size).Size());
	rect.Enlarge(Enlarge(rect, size).Size());
	sRectangle<T, Impl>.Enlarge(rect.Size());
}

template<typename T, saber::geometry::ImplKind Impl>
void RectangleScaleWork()
{
	const auto rect1 = sRectangle<T, Impl>;
	const auto size = saber::geometry::Size<T, Impl>{ 2, -3 };
	auto rect = Scale(rect1, size);

	rect.Scale(Scale(rect, size).Origin());
	rect.Scale(Scale(rect, size).Size());
	rect.Scale(Scale(rect, size).Origin());
	rect.Scale(Scale(rect, size).Size());
	rect.Scale(Scale(rect, size).Origin());
	rect.Scale(Scale(rect, size).Size());
	rect.Scale(Scale(rect, size).Origin());
	rect.Scale(Scale(rect, size).Size());
	sRectangle<T, Impl>.Scale(rect.Origin());
}

template<typename T, saber::geometry::ImplKind Impl>
void RectangleUnionWork()
{
	const auto rect1 = sRectangle<T, Impl>;
	const auto rect2 = saber::geometry::Rectangle<T, Impl>{ 2, -3, 10, 5 };
	auto rect = Union(rect1, rect2);

	rect.Union(Union(rect, rect2));
	rect.Union(Union(rect, rect1));
	rect.Union(Union(rect, rect2));
	rect.Union(Union(rect, rect1));
	rect.Union(Union(rect, rect2));
	rect.Union(Union(rect, rect1));
	rect.Union(Union(rect, rect2));
	rect.Union(Union(rect, rect1));
	sRectangle<T, Impl>.Union(rect);
}

template<typename T, saber::geometry::ImplKind Impl>
void RectangleIntersectWork()
{
	const auto rect1 = sRectangle<T, Impl>;
	const auto rect2 = saber::geometry::Rectangle<T, Impl>{ 2, -3, 10, 5 };
	auto rect = Intersect(rect1, rect2);

	rect.Intersect(Intersect(rect, rect2));
	rect.Intersect(Intersect(rect, rect1));
	rect.Intersect(Intersect(rect, rect2));
	rect.Intersect(Intersect(rect, rect1));
	rect.Intersect(Intersect(rect, rect2));
	rect.Intersect(Intersect(rect, rect1));
	rect.Intersect(Intersect(rect, rect2));
	rect.Intersect(Intersect(rect, rect1));
	sRectangle<T, Impl>.Intersect(rect);
}

template<typename T, saber::geometry::ImplKind Impl>
void RectangleIsOverlappingPointWork()
{
	const auto rect = sRectangle<T, Impl>;
	const auto point1 = saber::geometry::Point<T, Impl>{ 1, 1 };
	const auto point2 = saber::geometry::Point<T, Impl>{ 2, -3 };

	volatile bool result = false;
	result = IsOverlapping(rect, point1);
	result = rect.IsOverlapping(point2);
	result = IsOverlapping(rect, point2);
	result = rect.IsOverlapping(point1);
	result = IsOverlapping(rect, point1);
	result = rect.IsOverlapping(point2);
	result = IsOverlapping(rect, point2);
	result = rect.IsOverlapping(point1);
	result = IsOverlapping(rect, point1);
	(void)result;
}

template<typename T, saber::geometry::ImplKind Impl>
void RectangleIsOverlappingRectWork()
{
	const auto rect1 = sRectangle<T, Impl>;
	const auto rect2 = saber::geometry::Rectangle<T, Impl>{ 2, -3, 10, 5 };

	volatile bool result = false;
	result = IsOverlapping(rect1, rect2);
	result = rect1.IsOverlapping(rect2);
	result = IsOverlapping(rect2, rect1);
	result = rect2.IsOverlapping(rect1);
	result = IsOverlapping(rect1, rect2);
	result = rect1.IsOverlapping(rect2);
	result = IsOverlapping(rect2, rect1);
	result = rect2.IsOverlapping(rect1);
	result = IsOverlapping(rect1, rect2);
	(void)result;
}

template<typename T, saber::geometry::ImplKind Impl>
void RectangleIsEmptyWork()
{
	const auto rect1 = sRectangle<T, Impl>;
	const auto rect2 = saber::geometry::Rectangle<T, Impl>{ 2, -3, 10, 5 };

	volatile bool result = false;
	result = IsEmpty(rect1);
	result = IsEmpty(rect2);
	result = IsEmpty(rect1);
	result = IsEmpty(rect2);
	result = IsEmpty(rect1);
	result = IsEmpty(rect2);
	result = IsEmpty(rect1);
	result = IsEmpty(rect2);
	result = IsEmpty(rect1);
	(void)result;
}

template<typename T, saber::geometry::ImplKind Impl>
void RectangleCeilWork()
{
	auto rect = RoundCeil(sRectangle<T, Impl>);

	rect = RoundCeil(rect.RoundCeil());
	rect = RoundCeil(rect.RoundCeil());
	rect = RoundCeil(rect.RoundCeil());
	rect = RoundCeil(rect.RoundCeil());
	rect = RoundCeil(rect.RoundCeil());
	rect = RoundCeil(rect.RoundCeil());
	rect = RoundCeil(rect.RoundCeil());
	rect = RoundCeil(rect.RoundCeil());
	sRectangle<T, Impl> = RoundCeil(rect);
}

template<typename T, saber::geometry::ImplKind Impl>
void RectangleFloorWork()
{
	auto rect = RoundFloor(sRectangle<T, Impl>);

	rect = RoundFloor(rect.RoundFloor());
	rect = RoundFloor(rect.RoundFloor());
	rect = RoundFloor(rect.RoundFloor());
	rect = RoundFloor(rect.RoundFloor());
	rect = RoundFloor(rect.RoundFloor());
	rect = RoundFloor(rect.RoundFloor());
	rect = RoundFloor(rect.RoundFloor());
	rect = RoundFloor(rect.RoundFloor());
	sRectangle<T, Impl> = RoundFloor(rect);
}

template<typename T, saber::geometry::ImplKind Impl>
void RectangleNearestWork()
{
	auto rect = RoundNearest(sRectangle<T, Impl>);

	rect = RoundNearest(rect.RoundNearest());
	rect = RoundNearest(rect.RoundNearest());
	rect = RoundNearest(rect.RoundNearest());
	rect = RoundNearest(rect.RoundNearest());
	rect = RoundNearest(rect.RoundNearest());
	rect = RoundNearest(rect.RoundNearest());
	rect = RoundNearest(rect.RoundNearest());
	rect = RoundNearest(rect.RoundNearest());
	sRectangle<T, Impl> = RoundNearest(rect);
}

template<typename T, saber::geometry::ImplKind Impl>
void RectangleTruncWork()
{
	auto rect = RoundTrunc(sRectangle<T, Impl>);

	rect = RoundTrunc(rect.RoundTrunc());
	rect = RoundTrunc(rect.RoundTrunc());
	rect = RoundTrunc(rect.RoundTrunc());
	rect = RoundTrunc(rect.RoundTrunc());
	rect = RoundTrunc(rect.RoundTrunc());
	rect = RoundTrunc(rect.RoundTrunc());
	rect = RoundTrunc(rect.RoundTrunc());
	rect = RoundTrunc(rect.RoundTrunc());
	sRectangle<T, Impl> = RoundTrunc(rect);
}

TEMPLATE_TEST_CASE("saber::geometry::Rectangle", "[saber][benchmark][template]", int, float, double)
{
	using namespace saber::geometry;

	sRectangle<TestType, ImplKind::kScalar>.X(static_cast<TestType>(GauranteedNotConstexpr() + 0));
	sRectangle<TestType, ImplKind::kScalar>.Y(static_cast<TestType>(GauranteedNotConstexpr() + 1));
	sRectangle<TestType, ImplKind::kScalar>.Width(static_cast<TestType>(GauranteedNotConstexpr() + 2));
	sRectangle<TestType, ImplKind::kScalar>.Height(static_cast<TestType>(GauranteedNotConstexpr() + 3));

	sRectangle<TestType, ImplKind::kSimd>.X(static_cast<TestType>(GauranteedNotConstexpr() + 0));
	sRectangle<TestType, ImplKind::kSimd>.Y(static_cast<TestType>(GauranteedNotConstexpr() + 1));
	sRectangle<TestType, ImplKind::kSimd>.Width(static_cast<TestType>(GauranteedNotConstexpr() + 2));
	sRectangle<TestType, ImplKind::kSimd>.Height(static_cast<TestType>(GauranteedNotConstexpr() + 3));

	const auto rectScalarName = WorkloadName<TestType, ImplKind::kScalar>("Rectangle");
	const auto rectSimdName = WorkloadName<TestType, ImplKind::kSimd>("Rectangle");

	BENCHMARK(rectScalarName + "Translate()") { RectangleTranslateWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(rectSimdName + "Translate()") { RectangleTranslateWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(rectScalarName + "Enlarge()") { RectangleEnlargeWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(rectSimdName + "Enlarge()") { RectangleEnlargeWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(rectScalarName + "Scale()") { RectangleScaleWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(rectSimdName + "Scale()") { RectangleScaleWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(rectScalarName + "Union()") { RectangleUnionWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(rectSimdName + "Union()") { RectangleUnionWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(rectScalarName + "Intersect()") { RectangleIntersectWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(rectSimdName + "Intersect()") { RectangleIntersectWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(rectScalarName + "IsOverlapping(Point)") { RectangleIsOverlappingPointWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(rectSimdName + "IsOverlapping(Point)") { RectangleIsOverlappingPointWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(rectScalarName + "IsOverlapping(Rectangle)") { RectangleIsOverlappingRectWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(rectSimdName + "IsOverlapping(Rectangle)") { RectangleIsOverlappingRectWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(rectScalarName + "IsEmpty()") { RectangleIsEmptyWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(rectSimdName + "IsEmpty()") { RectangleIsEmptyWork<TestType, ImplKind::kSimd>(); };

	if constexpr (std::is_floating_point_v<TestType>)
	{
		// Rounding APIs are only available for floating point types
		BENCHMARK(rectScalarName + "RoundCeil()") { RectangleCeilWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(rectSimdName + "RoundCeil()") { RectangleCeilWork<TestType, ImplKind::kSimd>(); };

		BENCHMARK(rectScalarName + "RoundFloor()") { RectangleFloorWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(rectSimdName + "RoundFloor()") { RectangleFloorWork<TestType, ImplKind::kSimd>(); };

		BENCHMARK(rectScalarName + "RoundNearest()") { RectangleNearestWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(rectSimdName + "RoundNearest()") { RectangleNearestWork<TestType, ImplKind::kSimd>(); };

		BENCHMARK(rectScalarName + "RoundTrunc()") { RectangleTruncWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(rectSimdName + "RoundTrunc()") { RectangleTruncWork<TestType, ImplKind::kSimd>(); };
	}
};

template<typename T, saber::geometry::ImplKind Impl>
saber::geometry::Matrix<T, Impl> sMatrix{};

template<typename T, saber::geometry::ImplKind Impl>
void MatrixMakeIdentityWork()
{
	auto mat = saber::geometry::Matrix<T, Impl>::MakeIdentity();

	mat = saber::geometry::Matrix<T, Impl>::MakeIdentity();
	mat = saber::geometry::Matrix<T, Impl>::MakeIdentity();
	mat = saber::geometry::Matrix<T, Impl>::MakeIdentity();
	mat = saber::geometry::Matrix<T, Impl>::MakeIdentity();
	mat = saber::geometry::Matrix<T, Impl>::MakeIdentity();
	mat = saber::geometry::Matrix<T, Impl>::MakeIdentity();
	mat = saber::geometry::Matrix<T, Impl>::MakeIdentity();
	mat = saber::geometry::Matrix<T, Impl>::MakeIdentity();
	sMatrix<T, Impl> = mat;
}

template<typename T, saber::geometry::ImplKind Impl>
void MatrixMakeZeroWork()
{
	auto mat = saber::geometry::Matrix<T, Impl>::MakeZero();

	mat = saber::geometry::Matrix<T, Impl>::MakeZero();
	mat = saber::geometry::Matrix<T, Impl>::MakeZero();
	mat = saber::geometry::Matrix<T, Impl>::MakeZero();
	mat = saber::geometry::Matrix<T, Impl>::MakeZero();
	mat = saber::geometry::Matrix<T, Impl>::MakeZero();
	mat = saber::geometry::Matrix<T, Impl>::MakeZero();
	mat = saber::geometry::Matrix<T, Impl>::MakeZero();
	mat = saber::geometry::Matrix<T, Impl>::MakeZero();
	sMatrix<T, Impl> = mat;
}

template<typename T, saber::geometry::ImplKind Impl>
void MatrixMakeScaleWork()
{
	const auto point1 = saber::geometry::Point<T, Impl>{ 2, -3 };
	const auto point2 = saber::geometry::Point<T, Impl>{ -1, 4 };
	auto mat = saber::geometry::Matrix<T, Impl>::MakeScale(point1);

	mat = saber::geometry::Matrix<T, Impl>::MakeScale(point2);
	mat = saber::geometry::Matrix<T, Impl>::MakeScale(point1);
	mat = saber::geometry::Matrix<T, Impl>::MakeScale(point2);
	mat = saber::geometry::Matrix<T, Impl>::MakeScale(point1);
	mat = saber::geometry::Matrix<T, Impl>::MakeScale(point2);
	mat = saber::geometry::Matrix<T, Impl>::MakeScale(point1);
	mat = saber::geometry::Matrix<T, Impl>::MakeScale(point2);
	mat = saber::geometry::Matrix<T, Impl>::MakeScale(point1);
	sMatrix<T, Impl> = mat;
}

template<typename T, saber::geometry::ImplKind Impl>
void MatrixMakeTranslationWork()
{
	const auto point1 = saber::geometry::Point<T, Impl>{ 2, -3 };
	const auto point2 = saber::geometry::Point<T, Impl>{ -1, 4 };
	auto mat = saber::geometry::Matrix<T, Impl>::MakeTranslation(point1);

	mat = saber::geometry::Matrix<T, Impl>::MakeTranslation(point2);
	mat = saber::geometry::Matrix<T, Impl>::MakeTranslation(point1);
	mat = saber::geometry::Matrix<T, Impl>::MakeTranslation(point2);
	mat = saber::geometry::Matrix<T, Impl>::MakeTranslation(point1);
	mat = saber::geometry::Matrix<T, Impl>::MakeTranslation(point2);
	mat = saber::geometry::Matrix<T, Impl>::MakeTranslation(point1);
	mat = saber::geometry::Matrix<T, Impl>::MakeTranslation(point2);
	mat = saber::geometry::Matrix<T, Impl>::MakeTranslation(point1);
	sMatrix<T, Impl> = mat;
}

template<typename T, saber::geometry::ImplKind Impl>
void MatrixAddWork()
{
	const auto mat1 = sMatrix<T, Impl>;
	const auto mat2 = saber::geometry::Matrix<T, Impl>{ 2, -3, 1, 0, 2, -1 };
	auto mat = mat1;

	mat += mat2;
	mat += mat + mat2;
	mat += mat + mat1;
	mat += mat + mat2;
	mat += mat + mat1;
	mat += mat + mat2;
	mat += mat + mat1;
	mat += mat + mat2;
	mat += mat + mat1;
	sMatrix<T, Impl> += mat;
}

template<typename T, saber::geometry::ImplKind Impl>
void MatrixSubWork()
{
	const auto mat1 = sMatrix<T, Impl>;
	const auto mat2 = saber::geometry::Matrix<T, Impl>{ 2, -3, 1, 0, 2, -1 };
	auto mat = mat1;

	mat -= mat2;
	mat -= mat - mat2;
	mat -= mat - mat1;
	mat -= mat - mat2;
	mat -= mat - mat1;
	mat -= mat - mat2;
	mat -= mat - mat1;
	mat -= mat - mat2;
	mat -= mat - mat1;
	sMatrix<T, Impl> -= mat;
}

template<typename T, saber::geometry::ImplKind Impl>
void MatrixMulWork()
{
	const auto mat1 = sMatrix<T, Impl>;
	const auto mat2 = saber::geometry::Matrix<T, Impl>{ 2, -3, 1, 0, 2, -1 };
	auto mat = mat1;

	mat *= mat2;
	mat *= mat2;
	mat *= mat2;
	mat *= mat2;
	mat *= mat2;
	mat *= mat2;
	mat *= mat2;
	mat *= mat2;
	mat *= mat2;
	sMatrix<T, Impl> *= mat;
}

template<typename T, saber::geometry::ImplKind Impl>
void MatrixInvertWork()
{
	// TRICKY: Invert() is only numerically valid on non-singular matrices.
	// Reset to MakeIdentity() each iteration so every Invert() call acts on
	// a well-conditioned (orthogonal) matrix: inv(I) == I, inv(inv(M)) == M.
	auto mat = saber::geometry::Matrix<T, Impl>::MakeIdentity();

	mat.Invert();
	mat.Invert();
	mat.Invert();
	mat.Invert();
	mat.Invert();
	mat.Invert();
	mat.Invert();
	mat.Invert();
	mat.Invert();
	sMatrix<T, Impl> = mat;
}

template<typename T, saber::geometry::ImplKind Impl>
void MatrixMakeRotationWork()
{
	// MakeRotation() is only available for floating point types (calls std::sin/cos).
	// Varied angles prevent the compiler from folding repeated calls into one.
	const T angle1 = static_cast<T>(GauranteedNotConstexpr());
	const T angle2 = static_cast<T>(GauranteedNotConstexpr() + 1);
	auto mat = saber::geometry::Matrix<T, Impl>::MakeRotation(angle1);

	mat = saber::geometry::Matrix<T, Impl>::MakeRotation(angle2);
	mat = saber::geometry::Matrix<T, Impl>::MakeRotation(angle1);
	mat = saber::geometry::Matrix<T, Impl>::MakeRotation(angle2);
	mat = saber::geometry::Matrix<T, Impl>::MakeRotation(angle1);
	mat = saber::geometry::Matrix<T, Impl>::MakeRotation(angle2);
	mat = saber::geometry::Matrix<T, Impl>::MakeRotation(angle1);
	mat = saber::geometry::Matrix<T, Impl>::MakeRotation(angle2);
	mat = saber::geometry::Matrix<T, Impl>::MakeRotation(angle1);
	sMatrix<T, Impl> = mat;
}

TEMPLATE_TEST_CASE("saber::geometry::Matrix", "[saber][benchmark][template]", int, float, double)
{
	using namespace saber::geometry;

	sMatrix<TestType, ImplKind::kScalar>.M11(static_cast<TestType>(GauranteedNotConstexpr() + 0));
	sMatrix<TestType, ImplKind::kScalar>.M12(static_cast<TestType>(GauranteedNotConstexpr() + 1));
	sMatrix<TestType, ImplKind::kScalar>.M13(static_cast<TestType>(GauranteedNotConstexpr() + 2));
	sMatrix<TestType, ImplKind::kScalar>.M21(static_cast<TestType>(GauranteedNotConstexpr() + 3));
	sMatrix<TestType, ImplKind::kScalar>.M22(static_cast<TestType>(GauranteedNotConstexpr() + 4));
	sMatrix<TestType, ImplKind::kScalar>.M23(static_cast<TestType>(GauranteedNotConstexpr() + 5));

	sMatrix<TestType, ImplKind::kSimd>.M11(static_cast<TestType>(GauranteedNotConstexpr() + 0));
	sMatrix<TestType, ImplKind::kSimd>.M12(static_cast<TestType>(GauranteedNotConstexpr() + 1));
	sMatrix<TestType, ImplKind::kSimd>.M13(static_cast<TestType>(GauranteedNotConstexpr() + 2));
	sMatrix<TestType, ImplKind::kSimd>.M21(static_cast<TestType>(GauranteedNotConstexpr() + 3));
	sMatrix<TestType, ImplKind::kSimd>.M22(static_cast<TestType>(GauranteedNotConstexpr() + 4));
	sMatrix<TestType, ImplKind::kSimd>.M23(static_cast<TestType>(GauranteedNotConstexpr() + 5));

	const auto matrixNameScalar = WorkloadName<TestType, ImplKind::kScalar>("Matrix");
	const auto matrixNameSimd = WorkloadName<TestType, ImplKind::kSimd>("Matrix");

	BENCHMARK(matrixNameScalar + "MakeIdentity()") { MatrixMakeIdentityWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(matrixNameSimd + "MakeIdentity()") { MatrixMakeIdentityWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(matrixNameScalar + "MakeZero()") { MatrixMakeZeroWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(matrixNameSimd + "MakeZero()") { MatrixMakeZeroWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(matrixNameScalar + "MakeScale()") { MatrixMakeScaleWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(matrixNameSimd + "MakeScale()") { MatrixMakeScaleWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(matrixNameScalar + "MakeTranslation()") { MatrixMakeTranslationWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(matrixNameSimd + "MakeTranslation()") { MatrixMakeTranslationWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(matrixNameScalar + "operator+=") { MatrixAddWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(matrixNameSimd + "operator+=") { MatrixAddWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(matrixNameScalar + "operator-=") { MatrixSubWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(matrixNameSimd + "operator-=") { MatrixSubWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(matrixNameScalar + "operator*=") { MatrixMulWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(matrixNameSimd + "operator*=") { MatrixMulWork<TestType, ImplKind::kSimd>(); };

	BENCHMARK(matrixNameScalar + "Invert()") { MatrixInvertWork<TestType, ImplKind::kScalar>(); };
	BENCHMARK(matrixNameSimd + "Invert()") { MatrixInvertWork<TestType, ImplKind::kSimd>(); };

	if constexpr (std::is_floating_point_v<TestType>)
	{
		// MakeRotation() calls std::sin/cos and is only meaningful for floating point types
		BENCHMARK(matrixNameScalar + "MakeRotation()") { MatrixMakeRotationWork<TestType, ImplKind::kScalar>(); };
		BENCHMARK(matrixNameSimd + "MakeRotation()") { MatrixMakeRotationWork<TestType, ImplKind::kSimd>(); };
	}
};