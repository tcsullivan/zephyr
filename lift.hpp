/**
 * @file lift.hpp
 * @brief A system to handle all lift operations, using a quadrature
 * encoder. 
 */

#ifndef LIFT_HPP_
#define LIFT_HPP_

namespace lift {
	/**
	 * Initializes the lift system.
	 */
	void init(void);

	/**
	 * Sets the lift to the specified encoder value.
	 * This value is kept between -10 and the maximum height.
	 * @param target the target value to reach
	 * @see liftMaxHeight
	 */
	void set(const int& target);

	/**
	 * Increases the lift's target.
	 */
	void raise(void);

	/**
	 * Decreases the lift's target.
	 */
	void lower(void);
}

/**
 * Defines the highest encoder count the lift can reach.
 */
constexpr int liftMaxHeight = 110;

#endif // LIFT_HPP_
