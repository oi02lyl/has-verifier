/*
 * HashFunctions.h
 *
 *  Created on: Jul 19, 2016
 *      Author: lyl
 */

#ifndef UTILITY_H_
#define UTILITY_H_


namespace std {

typedef tuple<int, int, bool> term_t;
typedef tuple<int, int, int, bool> child_term_t;

struct key_hash: public std::unary_function<term_t, std::size_t> {
	std::size_t operator()(const term_t& k) const {
		return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
	}
};

struct key_equal: public std::binary_function<term_t, term_t, bool> {
	bool operator()(const term_t& v0, const term_t& v1) const {
		return (std::get<0>(v0) == std::get<0>(v1)
				&& std::get<1>(v0) == std::get<1>(v1)
				&& std::get<2>(v0) == std::get<2>(v1));
	}
};

struct key_hash2: public std::unary_function<child_term_t, std::size_t> {
	std::size_t operator()(const child_term_t& k) const {
		return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k)
				^ std::get<3>(k);
	}
};

struct key_equal2: public std::binary_function<child_term_t, child_term_t, bool> {
	bool operator()(const child_term_t& v0, const child_term_t& v1) const {
		return (std::get<0>(v0) == std::get<0>(v1)
				&& std::get<1>(v0) == std::get<1>(v1)
				&& std::get<2>(v0) == std::get<2>(v1)
				&& std::get<3>(v0) == std::get<3>(v1));
	}
};


}

#endif /* UTILITY_H_ */
