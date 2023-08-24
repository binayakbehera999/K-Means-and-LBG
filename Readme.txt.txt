========================================================================
    CONSOLE APPLICATION : K Means Project Overview
========================================================================

Objective - To classify the given universe into k clusters
Input - Universe.csv
Output - Codebook with k clusters

Instructions to execute the code:
1. Put a breakpoint after K_means function call in main().
2. Press F5
3. Output is printed in the console.

Description of the Code:

1. Generate 8 random numbers. Take delta as 0.001.
2. Intialise the codebook with the vectors at the 8 random nos.
3. For each vector in the universe:
	i. Find tokura distance with each vector in codebook.
	ii. Find the min distance. Put the vector into that cluster
4. Find the distortion by taking the average of the distortion of each bucket.
5. Update the new centroid.
6. Terminate when difference between previous distortion current distortion is less than delta.


========================================================================
    CONSOLE APPLICATION : LBG Project Overview
========================================================================

Objective - Find a optimal codebook with the given codebook
Input - Universe.csv
Output - Codebook with k clusters

Instructions to execute the code:
1. Put a breakpoint after LBG function call in main().
2. Press F5
3. Output is printed in the console.


Description of the Code:
1. intialise the codebook with centroid of universe.
2. Split the codebook -
	i. Run k means with the new centroids.
3. iterate 2 untill codebook size is k.