import requests

def fetch_keplerian_tle():
    url = "https://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle"
    response = requests.get(url)

    if response.status_code == 200:
        lines = [l.strip() for l in response.text.splitlines() if l.strip()]
        with open("starlink_processed.txt", "w") as out_file:
            for i in range(0, len(lines), 3):
                name = lines[i]
                # if "STARLINK" not in name: continue

                line1 = lines[i + 1]
                line2 = lines[i + 2]

                # Extract components needed for Keplerian Mechanics
                inclination_deg = float(line2[8:16])            # angle from equitorial plane to orbital plane
                raan_deg = float(line2[17:25])                  # angle from vernal equinox to ascending node
                eccentricity = float(f"0.{line2[26:33]}")       # Shape of orbit
                arg_perigee_deg = float(line2[34:42])           # Location of Pergiee (lowest point of orbit)
                mean_anomaly_deg = float(line2[43:51])          # Simplifed satelliate position
                mean_motion = float(line2[52:63])               # Revolutions per day
                epoch = float(line1[18:32])                     # Timestamp
                line = f"{name} {inclination_deg} {raan_deg} {eccentricity} {arg_perigee_deg} {mean_anomaly_deg} {mean_motion} {epoch}\n"
                out_file.write(line)

if __name__ == "__main__":
    fetch_keplerian_tle()

# TODO: Move above to CPP and have the app run a startup python script to pull
# in data from the API endpoint
# Check file last modified time before pulling from API