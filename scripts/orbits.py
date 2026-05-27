import numpy as np

# Constants
MU = 398600.4418  # Earth's gravitational parameter, km^3/s^2
R_EARTH = 6371.0  # Earth's radius in km
N_POINTS = 100    # points per orbit

# Parse file
def _is_float(s):
    try:
        float(s)
        return True
    except ValueError:
        return False

def parse_line(line):
    parts = line.strip().split()
    name = parts[0]
    numeric = [p for p in parts[1:] if _is_float(p)]
    inclination = np.radians(float(numeric[0]))
    raan = np.radians(float(numeric[1]))
    eccentricity = float(numeric[2])
    arg_perigee = np.radians(float(numeric[3]))
    mean_anomaly = np.radians(float(numeric[4]))
    mean_motion = float(numeric[5])  # revs per day
    epoch = float(numeric[6])
    return name, inclination, raan, eccentricity, arg_perigee, mean_anomaly, mean_motion

# Compute satellite position in ECI coordinates
def get_orbit_points(a, e, i, raan, argp, M0):
    points = []
    for k in range(N_POINTS):
        M = 2 * np.pi * k / N_POINTS  # Mean anomaly
        E = M  # Initial guess for eccentric anomaly

        # Solve Kepler's equation iteratively (Newton-Raphson)
        for _ in range(5):
            E = E - (E - e * np.sin(E) - M) / (1 - e * np.cos(E))

        # True anomaly
        nu = 2 * np.arctan2(np.sqrt(1 + e) * np.sin(E / 2),
                            np.sqrt(1 - e) * np.cos(E / 2))

        # Distance
        r = a * (1 - e * np.cos(E))

        # Perifocal coordinates
        x = r * np.cos(nu)
        y = r * np.sin(nu)
        z = 0

        # Rotate to ECI frame
        cos_raan, sin_raan = np.cos(raan), np.sin(raan)
        cos_argp, sin_argp = np.cos(argp), np.sin(argp)
        cos_i, sin_i = np.cos(i), np.sin(i)

        R = np.array([
            [cos_raan*cos_argp - sin_raan*sin_argp*cos_i, -cos_raan*sin_argp - sin_raan*cos_argp*cos_i, sin_raan*sin_i],
            [sin_raan*cos_argp + cos_raan*sin_argp*cos_i, -sin_raan*sin_argp + cos_raan*cos_argp*cos_i, -cos_raan*sin_i],
            [sin_argp*sin_i, cos_argp*sin_i, cos_i]
        ])

        r_eci = R @ np.array([x, y, z])
        points.append(r_eci)
    return np.array(points)

# Convert mean motion to semi-major axis
def mean_motion_to_a(n):
    n_rad = n * 2 * np.pi / 86400  # revs/day to rad/s
    a = (MU / n_rad**2) ** (1/3)   # km
    return a

# Load and process satellites
satellite_vertices = []

with open("starlink_processed.txt", "r") as file:
    for line in file:
        name, i, raan, e, argp, M0, n = parse_line(line)
        a = mean_motion_to_a(n)
        orbit = get_orbit_points(a, e, i, raan, argp, M0)
        satellite_vertices.append(orbit)

# Dump to file
with open("precomputed_orbits.bin", "wb") as out:
    with open("starlink_processed.txt", "r") as file:
        for line in file:
            name, i, raan, e, argp, M0, n = parse_line(line)
            a = mean_motion_to_a(n)
            orbit = get_orbit_points(a, e, i, raan, argp, M0)

            # Write mean_motion as a single float
            np.array([n], dtype=np.float32).tofile(out)

            # Write orbit vertices (100 x 3 floats)
            orbit.astype(np.float32).tofile(out)

