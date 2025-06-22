import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
from matplotlib.offsetbox import OffsetImage, AnnotationBbox

# Data
# team_a = [66, 452, 1132, 237, 1132, 361, 228, 395, 66]
# team_b = [65, 524, 950, 290, 950, 399, 363, 385, 65]
team_a = [51, 1371, 5398, 405, 5398, 1169, 623, 1332, 51]
team_b = [55, 1595, 4080, 671, 4080, 1247, 918, 1057, 55]

# Patterns and their corresponding images (make sure these images exist in your working directory)
pattern_images = ["pattern1.png", "pattern12.png", "pattern121.png", "pattern123.png", 
                  "pattern1212.png", "pattern1213.png", "pattern1231.png", "pattern1232.png", "pattern1234.png"]

# Bar width and positions
x = np.arange(len(pattern_images))  # X-axis positions
bar_width = 0.35

# Create the figure and axes
fig, ax = plt.subplots(figsize=(12, 7))

# Plot bars
bars_team_a = ax.bar(x - bar_width/2, team_a, bar_width, label='Team A', color='orange', alpha=0.7)
bars_team_b = ax.bar(x + bar_width/2, team_b, bar_width, label='Team B', color='black', alpha=0.7)

# Add labels, title, and legend
ax.set_ylabel('Number of Occurrences', fontsize=12)
ax.set_xlabel('Pattern type', fontsize = 12)
ax.legend()

# Remove default x-tick labels
ax.set_xticks([])

# Add images as x-axis labels
for i, img_path in enumerate(pattern_images):
    img = mpimg.imread(img_path)  # Load image
    imagebox = OffsetImage(img, zoom=0.5)  # Adjust zoom for appropriate size
    ab = AnnotationBbox(imagebox, (i, 0), frameon=False, box_alignment=(0.5, 1.5))
    ax.add_artist(ab)

# Add data labels above bars
for bar in bars_team_a:
    ax.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 10, str(bar.get_height()), ha='center', fontsize=10)
for bar in bars_team_b:
    ax.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 10, str(bar.get_height()), ha='center', fontsize=10)

ax.xaxis.set_label_coords(0.5, -0.15)  

# Show plot
plt.tight_layout()
plt.show()
