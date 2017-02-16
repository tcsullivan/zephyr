/**
 * @file claw.hpp
 * @brief A system to handle all claw operations.
 */

#ifndef CLAW_HPP_
#define CLAW_HPP_

namespace claw {
	/**
	 * Initializes the claw system.
	 */
	void init(void);

	/**
	 * Opens the claw, using pre-defined open time.
	 * @see openTime
	 */
	void open(void);

	/**
	 * Closes the claw using the pre-defined close time.
	 * @see closeTime
	 */
	void close(void);

	/**
	 * Opens the claw, but then leaves the motor powered so that it can hold
	 * an object.
	 */
	void hold(void);

	/**
	 * Runs a close-open combination to release the claw from its locked
	 * position on the lift.
	 */
	void unhook(void);
}

/**
 * Defines how many milliseconds it takes for the claw to open.
 */
constexpr unsigned int openTime = 700;

/**
 * Defines how many milliseconds it takes for the claw to close.
 */
constexpr unsigned int closeTime = 900;

#endif // CLAW_HPP_
