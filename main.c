#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

// Number of data points
#define N 10000

// Number of clusters
#define K 5

// Number of features (dimensionality)
#define D 2



#define printCentroids() \
    for (int i = 0; i < K; i++) \
    printf (" Print %f,%f\n", centroids[i][0], centroids[i][1]);

float square(float b);
// Matrix with data from csv
float Data[N][D];
// Matrix for the different distances
float distance [N][K];

// Holds the coordinates of the centroids


float centroids [K][D] =
{
    {-0.357, -0.253},
    {-0.055, 4.392},
    {2.674, -0.001},
    {1.044, -1.251},
    {-1.495, -0.090}
};


float temp_centroids[K][D];
char counts[K];

int cluster_assigment[N];
int old_cluster_assigment[N];

int converge;

int n, k, i, d =0;

int cnt = 0;

int main(void)
{

    
    printCentroids()

    // Read the CSV
    FILE *in=fopen("/input.csv","r");         /* open file on command line */
    
    // Safe side
    if(in==NULL)
    {
        perror("File open error");
        return(1);
    }
    
    // x holds the 1st dimension of the data points
    float x;
    // y holds the 2nd dimension of the data points
    float y;
    
    
    // For all the elements of the csv (i.e. all the rows)
    for (n = 0; n < N; n++)
    {
        // Read the line which has format "float comma float"
        fscanf(in, "%f,%f", &x, &y);
        
        // Debug printf
        //printf("%f,  %f \n", x,y);
        
        // and save the features to the main Data matrix
        Data[n][0] = x;
        Data[n][1] = y;
    }

    // Close the csv and move on with Kmeans
    fclose(in);
    
    
    // -------- K means Starts here
    
    // It will converge only if the new centroids are not different
    // from the old centroids
    while( converge == 0)
    {
        
        // For all N entries calc. the min distance for each cluster
        for( n=0;  n < N; n++)
        {
            // Init. to a max
            float min_distance = FLT_MAX;
            
            // for all 5 clusters
            for( k=0; k < K; k++ )
            {
                // Init.
                float feature_distance = 0;
                
                // For all features
                for (d=0; d <D; d++)
                {
                    // Calc the square temrs/feature which will be used for the Euclidian distance
                    feature_distance += square ( centroids[k][d] - Data[n][d] );
                }
                
                // Euclidian distance =  sqrt[ sqr(x1 - x2) + sqr(y1-y2) ]
                distance[n][k] = sqrt(feature_distance);
                
                //  Each data point is assigned to one of the K clusters with respect its
                // minimum distance
                if(distance[n][k] < min_distance)
                {
                    min_distance = distance[n][k];
                    
                    // Save the index of the cluster for grouping purposes
                    cluster_assigment[n] = k;
                }
            }
            
            // Pick up the cluster index
            int cluster_idx = cluster_assigment[n];
            
            for (d=0; d <D; d++)
            {
                // Accumulated sum in order to find the mean per cluster
                // per itteration per feature
                temp_centroids[ cluster_idx ][d] += Data[n][d];
            }
            
            // Number of data points per cluster
            counts[cluster_idx] +=1;
            
        }
        
        // For all means update the centroid/mean
        for(k=0; k < K; k++)
        {
            // For all features
            for (d=0; d <D; d++)
            {
                // Update the centroids with respect to the new means
                centroids[k][d] = temp_centroids[k][d] / counts[k];
                
                // These two variables served their purpose so reset
                // for the next itteration
                temp_centroids[k][d] = 0;
                counts[k] = 0 ;
            }
            
        }
        
        // Init.
        converge = 1;
        
        // For all data  entries
        for(n=0; n<N; n++)
        {
            // If nothing changed it means that the algorithm is converged
            if( cluster_assigment[n] != old_cluster_assigment[n] )
            {
                converge = 0;
            }
            
            // Save clusters for the forthcoming loop
            old_cluster_assigment[n] = cluster_assigment[n];
        }
        
        cnt++;
        
        
        
    }//-------- K-Means ends here
    
    printCentroids()
    
    
    
    
    FILE *f = fopen("kmeans/OUTPUT.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return(1);
    }
    
    
    for (n=0; n<N ; n++)
    {
        
        fprintf(f, "%d \n", cluster_assigment[n]);

    }
    fclose(f);
    
    
    return(0);
}

// Calcs the square of a variable
float square(float b)
{
    float z;
    z = b*b;
   // printf("%f %f",z ,b);
    return(z);
}





