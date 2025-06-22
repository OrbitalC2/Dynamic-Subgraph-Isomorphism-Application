# DYNAMIC PASS-NETWORK PATTERNS

This project converts a football match’s raw pass log (passes.csv) into a
series of directed, time-aware graphs and uses Oberoi et al.’s **Dynamic
Sub-Graph Isomorphism (DSISO)** algorithm to count how often each attacking
pattern (motif) appears.  A small Python helper then plots a bar chart
comparing the two teams’ pattern frequencies.

## Why it matters

Coaches talk about “third-man runs”, “diamonds”, “overlaps”, but verifying how
often they really happen is tedious.  By framing every sliding window of passes
as a graph and running DSISO we can back up the eye-test with hard numbers.

---

## REPOSITORY CONTENTS

passes.csv               Raw event log: time\_ms, team\_id, passer, receiver, …
generateTextFiles.cpp    C++17 tool that reads passes.csv, walks a sliding
window (default 4 passes / 10 s), and writes each
window to nodes.txt + edges.txt in the format DSISO
expects.
launcher.sh              Bash orchestrator: cleans out/, compiles the converter,
feeds every window to the DSISO binary, and merges the
results into counts.csv.
graphs.py                Python script that draws a side-by-side bar chart of
pattern counts for Team A vs Team B.
oberoi/                  Git submodule – original Dynamic Sub-Graph Isomorphism
implementation.  Built as the executable dsiso.
out/                     Auto-generated directory holding intermediate graph
files and the final counts.csv.

---

## BUILD AND RUN

1. Clone with submodule
   git clone --recursive [https://github.com/](https://github.com/)<user>/pass-pattern-dsiso.git
   cd pass-pattern-dsiso

2. Build the converter and DSISO
   mkdir build && cd build
   cmake ..
   make -j          # generates generateTextFiles and dsiso

3. Launch the pipeline
   ./launcher.sh passes.csv   # writes out/counts.csv and progress messages

4. Visualise the results
   python graphs.py           # opens a Matplotlib window

---

## DATA FORMAT EXPECTATIONS

passes.csv must contain at least these columns:

time\_ms         integer   milliseconds since kick-off
team\_id         integer   1 = Team A, 2 = Team B
from\_player\_id  integer   passer ID
to\_player\_id    integer   receiver ID

Extra columns (x, y, pressure, etc.) are ignored.

---

## SLIDING-WINDOW PARAMETERS

Inside generateTextFiles.cpp you can change

WINDOW\_SIZE\_PASSES   how many consecutive passes form one graph (default 4)
WINDOW\_TIME\_MS       maximum span of those passes in milliseconds (default 10 000)

Every window is exported, processed by dsiso, and any matched motif IDs are
logged.

---

## DEPENDENCIES

• C++17 compiler: GCC 11+, Clang 14+, or MSVC 19.3x
• CMake 3.18 or newer
• Python 3.8+ with numpy and matplotlib
(pip install -r requirements.txt)
• Bash / POSIX coreutils (Windows users can run under WSL or Git Bash)

---

## EXTENDING THE PROJECT

– Swap Bash pipes for a light message queue to parallelise DSISO calls.
– Feed multiple matches to build a season-long motif table.
– Store counts in SQLite and expose a small Flask dashboard.
– Experiment with different window sizes (e.g. three-pass triangles).

---

## REFERENCE

A. K. Oberoi et al., “Efficient Detection of Subgraph Isomorphism in Dynamic
Graphs”, Journal of Graph Algorithms and Applications, 2018.

---

## LICENSE

MIT – free for academic or commercial use.  If you publish work based on this
pipeline, please credit Oberoi 2018 for the DSISO algorithm.

