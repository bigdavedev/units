# units

[![Build Status](https://travis-ci.org/bigdavedev/units.svg?branch=master)](https://travis-ci.org/bigdavedev/units)
[![Build status](https://ci.appveyor.com/api/projects/status/nw99lno48xefeg87/branch/master?svg=true)](https://ci.appveyor.com/project/bigdavedev/units/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/bigdavedev/units/badge.svg?branch=master)](https://coveralls.io/github/bigdavedev/units?branch=master)

## Purpose
This units library strives to provide typesafe code for representing any kind of numerical unit (distance, volume, mass, etc) in C++ that are interoperable with the existing std::chrono implementation. Units strives to pure compile time where that is appropriate. In addition, use of user-defined literals are used to allow for more human-like representation of units.

    auto distance = 1_m;

yieds:

    units::distance<double> distance

## Examples
Create a unit representing 2500 metres:

    auto distance = 2500_m;
    std::cout << distance.count() << std::endl; // 2500.0

Add 31 metres:

    distance += 31_m;
    std::cout << distance.count() << std::endl; // 2531.0

Convert to kilometres:

    auto kilometres = units::distance_cast<units::kilometres>(distance);
    std::cout << kilometres.count() << std::endl; // 2.531

Get fractional part of kilometres as metres:

    auto metres = kilometres % 1_km;
    std::cout << metres.count() << std::endl; // 531.0

Convert to feet:

    auto feet = units::distance_cast<units::feet>(metres);
	std::cout << feet.count() << std::endl; // 1742.13

Note that the default underlying type of the units class is `double` but you can easily change that yourself:

    using metres_i = units::distance<int>;
	metres_i foo{1234};
	std::cout << foo.count() << std::endl; // 1234
