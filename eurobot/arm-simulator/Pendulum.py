import numpy as np


EARTH_GRAVITY_CONSTANT = 9.81  # m/s^2


class Pendulum:
    def __init__(self, length, mass):
        self.length = length
        self.mass = mass
        self.q = 0
        self.q_dot = 0

    def reset(self, q, q_dot):
        """
        Set the angle and angular velocity of the pendulum
        """
        self.q = q
        self.q_dot = q_dot

    def step(self, dt):
        """
        Compute a forward step on the internal model.
        Updates the position and velocity of the pendulum
        """
        self.q_dot = self.q_dot + self._q_ddot() * dt
        self.q = self.q + self.q_dot * dt

    def position(self):
        """
        Position of the tip of the pendulum
        """
        x = self.length * np.sin(self.q)
        y = - self.length * np.cos(self.q)
        return x, y

    def velocity(self):
        """
        Velocity of the tip of the pendulum
        """
        v_x = self.length * np.cos(self.q) * self.q_dot
        v_y = self.length * np.sin(self.q) * self.q_dot
        return v_x, v_y

    def kinetic_energy(self):
        return 0.5 * self.mass * (self.length * self.q_dot) ** 2

    def potential_energy(self):
        return self.mass * EARTH_GRAVITY_CONSTANT * self.length * (1 - np.cos(self.q))

    def _q_ddot(self):
        return - (EARTH_GRAVITY_CONSTANT / self.length) * np.sin(self.q)
