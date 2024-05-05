#pragma once

template<typename T>
static T
max(T a, T b) {
	return a > b ? a : b;
}

template<typename T>
static T
min(T a, T b) {
	return a < b ? a : b;
}