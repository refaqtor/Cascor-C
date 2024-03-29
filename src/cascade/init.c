/*	CMU Cascade Neural Network Simulator (CNNS)
    General Allocation and Initialization Functions

    v1.1
    Ian Chiu (ichiu@andrew.cmu.edu)
    7/17/2018

    Improving readability and maintainability


    v1.0
    Matt White  (mwhite+@cmu.edu)
    December 5, 1994

    These are functions for allocating and initializing the data structures
    used for training and simulating the network.
    */

#include <string.h>

#include "toolkit.h"
#include "cascade.h"


/*  BUILD NET -  Create a network with the parameters specified and initialize
    the fields to appropriate values.
    */

net_t *build_net  ( char *name, int Ninputs, int Noutputs, int maxNewUnits,
        float weightRange, float sigMax, float sigMin,
        boolean recurrent )
{
    net_t *temp;
    int   maxUnits,
          i,j;
    char  *fn_name = "Build New Network";

    temp = (net_t *) alloc_mem( 1, sizeof( net_t ), fn_name );

    temp->name     = strdup( name );
    temp->filename = NULL;
    temp->descript = NULL;

    temp->epochsTrained = 0;
    temp->Nunits        = Ninputs+1;
    temp->Ninputs       = Ninputs;
    temp->Noutputs      = Noutputs;
    temp->NhiddenUnits  = 0;
    temp->maxNewUnits   = maxNewUnits;

    temp->recurrent     = recurrent;

    temp->sigmoidMax    = sigMax;
    temp->sigmoidMin    = sigMin;

    temp->inputMap      = NULL;
    temp->outputMap     = NULL;
    temp->next          = NULL;

    maxUnits           = Ninputs + maxNewUnits + 1;
    temp->tempValues   = (float *)alloc_mem  ( maxUnits, sizeof( float ),
                                               fn_name );
    temp->values       = temp->tempValues;
    temp->weights      = (float **)alloc_mem ( maxUnits, sizeof( float * ),
                                               fn_name );
    temp->unitTypes    = (node_t *)alloc_mem ( maxUnits, sizeof( node_t ),
                                               fn_name );
    for  ( i = 0 ; i < maxUnits ; i++ ) {
        if  ( i < temp->Nunits ) {
            temp->weights[i] = NULL;
        } else  {
            temp->weights[i] = (float *)alloc_mem (i+recurrent, sizeof( float ),
                                                   fn_name);
            temp->unitTypes[i] = SIGMOID;
        }
    }

    temp->outValues   = (float *)alloc_mem  ( Noutputs, sizeof( float ),
                                              fn_name );
    temp->outWeights  = (float **)alloc_mem ( Noutputs, sizeof( float * ),
                                              fn_name );
    temp->outputTypes = (node_t *)alloc_mem ( Noutputs, sizeof( node_t ),
                                              fn_name );
    for  ( i = 0 ; i < Noutputs ; i++ )  {
        temp->outWeights[i] = (float *)alloc_mem ( maxUnits, sizeof( float ),
                                                   fn_name );
        for  ( j = 0 ; j < Ninputs+1 ; j++ ) {
            temp->outWeights[i][j] = random_weight( weightRange );
        }
        temp->outputTypes[i] = SIGMOID;
    }

    return temp;
}


/*  REALLOC NET -  Reallocates memory for a network so that additional units
    may be added.  This does not affect other structures than the net and so
    cannot be used during training.
    */

void realloc_net ( net_t *net, int newUnits )
{
    int     i, j,
            maxUnits,
            Ninputs;
    char    *fn_name = "Reallocate Network";

    maxUnits = net->Nunits+net->maxNewUnits+newUnits;
    Ninputs  = net->Ninputs;

    net->maxNewUnits += newUnits;

    net->tempValues = (float *)realloc_mem( net->tempValues, maxUnits,
            sizeof( float ), fn_name );
    net->values     = net->tempValues;
    net->weights    = (float **)realloc_mem( net->weights, maxUnits,
            sizeof( float * ), fn_name );
    net->unitTypes  = (node_t *)realloc_mem( net->unitTypes, maxUnits,
            sizeof( node_t ), fn_name );

    for ( i = maxUnits-newUnits ; i < maxUnits ; i++ ) {
        net->weights[i] = (float *)alloc_mem (i+net->recurrent,sizeof( float ),
                                              fn_name);
    }

    for ( i = 0 ; i < net->Noutputs ; i++ ) {
        net->outWeights[i] = (float *)realloc_mem( net->outWeights[i], maxUnits,
                sizeof( float ), fn_name );
    }
}


/*	INIT NET -  Reset a network to its initial state.  Memory remains
    allocated as it was previously.  Weights are reinitialized to be
    within +/-weightRange.
    */

void init_net ( net_t *net, float weightRange )
{
    int i,j;

    net->maxNewUnits   += net->NhiddenUnits;
    net->Nunits        -= net->NhiddenUnits;

    net->epochsTrained = 0;
    net->NhiddenUnits  = 0;
    for  ( i = 0 ; i < net->Noutputs ; i++ ) {
        for  ( j = 0 ; j < net->Ninputs+1 ; j++ ) {
            net->outWeights[i][j] = random_weight( weightRange );
        }
    }
}


/*	FREE NET -  Deallocate the memory associated with a network.
*/

void free_net ( net_t **net )
{
    int maxUnits,
        i;

    (*net)->name     = free_mem( (*net)->name );
    (*net)->filename = free_mem( (*net)->filename );
    (*net)->descript = free_mem( (*net)->descript );

    (*net)->tempValues = free_mem( (*net)->tempValues );
    (*net)->unitTypes  = free_mem( (*net)->unitTypes );
    maxUnits = (*net)->Nunits + (*net)->maxNewUnits;
    for  ( i = 0 ; i < maxUnits ; i++ ) {
        (*net)->weights[i] = free_mem( (*net)->weights[i] );
    }
    (*net)->weights = free_mem( (*net)->weights );

    (*net)->outValues = free_mem( (*net)->outValues );
    (*net)->outputTypes = free_mem( (*net)->outputTypes );
    for  ( i = 0 ; i < (*net)->Noutputs ; i++ ) {
        (*net)->outWeights[i] = free_mem( (*net)->outWeights[i] );
    }
    (*net)->outWeights = free_mem( (*net)->outWeights );

    *net = free_mem( *net );
}


/*  BUILD PARM -  Allocate a network training parameter table and initialize
    its values to the their specified defaults.
    */

train_parm_t *build_parm ( void )
{
    train_parm_t *temp;
    char         *fn_name = "Build Network Parameter Table";

    temp = (train_parm_t *)alloc_mem ( 1, sizeof( train_parm_t ), fn_name );

    temp->maxNewUnits                   = 50;
    temp->validationPatience            = 8;
    temp->Ncand                         = 8;

    temp->outPrimeOffset                = 0.1;
    temp->weightRange                   = 1.0;
    temp->indexThreshold                = 0.2;
    temp->scoreThreshold                = 0.4;
    temp->sigMax                        = DEF_SIGMAX;
    temp->sigMin                        = DEF_SIGMIN;

    temp->overshootOK                   = FALSE;
    temp->useCache                      = TRUE;
    temp->test                          = TRUE;
    temp->validate                      = TRUE;
    temp->recurrent                     = FALSE;

    temp->candType                      = SIGMOID;
    temp->algorithm                     = CASCOR;
    temp->errorMeasure                  = BITS;

    temp->candInUpdate.epsilon          = 100.0;
    temp->candInUpdate.mu               = 2.0;
    temp->candInUpdate.decay            = 0.000;
    temp->candOutUpdate.epsilon         = 100.0;
    temp->candOutUpdate.mu              = 2.0;
    temp->candOutUpdate.decay           = 0.0;
    temp->outputUpdate.epsilon          = 1.0;
    temp->outputUpdate.mu               = 2.0;
    temp->outputUpdate.decay            = 0.000;

    temp->candidateParm.epochs          = 200;
    temp->candidateParm.patience        = 12;
    temp->candidateParm.changeThreshold = 0.03;
    temp->outputParm.epochs             = 200;
    temp->outputParm.patience           = 12;
    temp->outputParm.changeThreshold    = 0.01;

    return temp;
}


/*  BUILD TRAIN DATA -  Build a structure to store information used to modify
    the weight values of the network.  Also allocates memory for the
    computation cache.
    */

train_data_t *build_train_data  ( net_t *net, train_parm_t *parms, int Npts )
{
    train_data_t *temp;
    int          Ncand,
                 Noutputs,
                 maxUnits,
                 NinConn,
                 i,j;
    char         *fn_name = "Build Network Training Data";


    Ncand    = parms->Ncand;
    Noutputs = net->Noutputs;
    maxUnits = net->Nunits + net->maxNewUnits;
    NinConn  = maxUnits + net->recurrent;

    temp = (train_data_t *)alloc_mem ( 1, sizeof( train_data_t ), fn_name );

    if  ( parms->useCache )  {
        temp->cachePts = Npts;
        parms->useCache = build_cache ( maxUnits, Noutputs, Npts,
                &(temp->valCache), &(temp->errCache) );
    }

    temp->candScores   = (float *)alloc_mem (Ncand, sizeof( float ), fn_name);
    temp->candValues   = (float *)alloc_mem (Ncand, sizeof( float ), fn_name);
    temp->candSumVals  = (float *)alloc_mem (Ncand, sizeof( float ), fn_name);
    temp->candCorr     = (float **)alloc_mem (Ncand, sizeof( float * ),fn_name);
    temp->candPrevCorr = (float **)alloc_mem (Ncand, sizeof( float * ),fn_name);
    temp->candTypes    = (node_t *)alloc_mem (Ncand, sizeof( node_t ), fn_name);
    if  ( parms->recurrent )  {
        temp->candDVdW     = (float **)alloc_mem (Ncand, sizeof( float * ),
                                                  fn_name);
        temp->candPrevValues  = (float *)alloc_mem (Ncand, sizeof( float ),
                                                    fn_name);
    }
    temp->candIn.weights = (float **)alloc_mem (Ncand, sizeof( float * ),
                                                fn_name);
    temp->candIn.deltas  = (float **)alloc_mem (Ncand, sizeof( float * ),
                                                fn_name);
    temp->candIn.slopes  = (float **)alloc_mem (Ncand, sizeof( float * ),
                                                fn_name);
    temp->candIn.pSlopes = (float **)alloc_mem (Ncand, sizeof( float * ),
                                                fn_name);

    temp->candOut.weights = (float **)alloc_mem (Ncand, sizeof( float * ),
                                                 fn_name);
    temp->candOut.deltas  = (float **)alloc_mem (Ncand, sizeof( float * ),
                                                 fn_name);
    temp->candOut.slopes  = (float **)alloc_mem (Ncand, sizeof( float * ),
                                                 fn_name);
    temp->candOut.pSlopes = (float **)alloc_mem (Ncand, sizeof( float * ),
                                                 fn_name);

    temp->output.weights = NULL;
    temp->output.deltas  = (float **)alloc_mem (Noutputs, sizeof( float * ),
                                                fn_name);
    temp->output.slopes  = (float **)alloc_mem (Noutputs, sizeof( float * ),
                                                fn_name);
    temp->output.pSlopes = (float **)alloc_mem (Noutputs, sizeof( float * ),
                                                fn_name);

    for  ( i = 0 ; i < Ncand ; i++ )  {
        temp->candCorr[i]     = (float *)alloc_mem (Noutputs, sizeof( float ),
                                                    fn_name);
        temp->candPrevCorr[i] = (float *)alloc_mem (Noutputs, sizeof( float ),
                                                    fn_name);
        if  ( parms->recurrent )
            temp->candDVdW[i]   = (float *)alloc_mem (maxUnits, sizeof( float ),
                                                      fn_name);

        temp->candIn.weights[i] = (float *)alloc_mem (NinConn,sizeof( float ),
                                                      fn_name);
        temp->candIn.deltas[i]  = (float *)alloc_mem (NinConn,sizeof( float ),
                                                      fn_name);
        temp->candIn.slopes[i]  = (float *)alloc_mem (NinConn,sizeof( float ),
                                                      fn_name);
        temp->candIn.pSlopes[i] = (float *)alloc_mem (NinConn,sizeof( float ),
                                                      fn_name);

        temp->candOut.weights[i] = (float *)alloc_mem(Noutputs,sizeof( float ),
                                                      fn_name);
        temp->candOut.deltas[i]  = (float *)alloc_mem(Noutputs,sizeof( float ),
                                                      fn_name);
        temp->candOut.slopes[i]  = (float *)alloc_mem(Noutputs,sizeof( float ),
                                                      fn_name);
        temp->candOut.pSlopes[i] = (float *)alloc_mem(Noutputs,sizeof( float ),
                                                      fn_name);
    }

    for  ( i = 0 ; i < Noutputs ; i++ )  {
        temp->output.deltas[i]  = (float *)alloc_mem (maxUnits,sizeof( float ),
                                                      fn_name);
        temp->output.slopes[i]  = (float *)alloc_mem (maxUnits,sizeof( float ),
                                                      fn_name);
        temp->output.pSlopes[i] = (float *)alloc_mem (maxUnits,sizeof( float ),
                                                      fn_name);

        for  ( j = 0 ; j < maxUnits ; j++ )  {
            temp->output.deltas[i][j]  = 0.0;
            temp->output.slopes[i][j]  = 0.0;
            temp->output.pSlopes[i][j] = 0.0;
        }
    }

    temp->outScaledEps         = parms->outputUpdate.epsilon / Npts;
    temp->output.shrinkFactor  = parms->outputUpdate.mu /
        (parms->outputUpdate.mu + 1.0);
    temp->candIn.shrinkFactor  = parms->candInUpdate.mu /
        (parms->candInUpdate.mu + 1.0);
    temp->candOut.shrinkFactor = parms->candOutUpdate.mu /
        (parms->candOutUpdate.mu + 1.0);

    return temp;
}


/*	FREE TRAIN DATA -  Deallocate memory allocated for the training of a
    network.  In addition, any cache is deallocated.
    */

void  free_train_data  ( train_data_t **data, net_t *net, train_parm_t *parm )
{
    int Ncand,
        Noutputs,
        maxUnits,
        NinConn,
        i;

    Ncand    = parm->Ncand;
    Noutputs = net->Noutputs;

    if  ( parm->useCache )
        free_cache(&((*data)->valCache),&((*data)->errCache),(*data)->cachePts);

    free_mem( (*data)->candScores );
    free_mem( (*data)->candValues );
    free_mem( (*data)->candSumVals );
    free_mem( (*data)->candTypes );

    for  ( i = 0 ; i < Ncand ; i++ )  {
        free_mem( (*data)->candCorr[i] );
        free_mem( (*data)->candPrevCorr[i] );

        free_mem( (*data)->candIn.weights[i] );
        free_mem( (*data)->candIn.deltas[i] );
        free_mem( (*data)->candIn.slopes[i] );
        free_mem( (*data)->candIn.pSlopes[i] );

        free_mem( (*data)->candOut.weights[i] );
        free_mem( (*data)->candOut.deltas[i] );
        free_mem( (*data)->candOut.slopes[i] );
        free_mem( (*data)->candOut.pSlopes[i] );
    }

    for  ( i = 0 ; i < Noutputs ; i++ )  {
        free_mem( (*data)->output.deltas[i] );
        free_mem( (*data)->output.slopes[i] );
        free_mem( (*data)->output.pSlopes[i] );
    }

    free_mem( (*data)->candCorr );
    free_mem( (*data)->candPrevCorr );
    free_mem( (*data)->candIn.weights );
    free_mem( (*data)->candIn.deltas );
    free_mem( (*data)->candIn.slopes );
    free_mem( (*data)->candIn.pSlopes );
    free_mem( (*data)->candOut.weights );
    free_mem( (*data)->candOut.deltas );
    free_mem( (*data)->candOut.slopes );
    free_mem( (*data)->candOut.pSlopes );
    free_mem( (*data)->output.deltas );
    free_mem( (*data)->output.slopes );
    free_mem( (*data)->output.pSlopes );

    if  ( parm->recurrent )  {
        free_mem( (*data)->candDVdW );
        free_mem( (*data)->candPrevValues );
    }

    *data = free_mem( *data );
}


/*	INIT CAND -  Initializes the candidate units in 'tData' for another
    round of training.  Call this function before you begin training a new
    set of candidates.
    */

void init_cand ( train_data_t *tData, int Ncand, int Noutputs, int Nunits,
        boolean recurrent, float weightRange, node_t candType )
{
    int i,j;

    for  ( i = 0 ; i < Ncand ; i++ )  {
        tData->candValues[i]  = 0.0;
        if  ( recurrent ) {
            tData->candPrevValues[i] = 0.0;
        }
        tData->candSumVals[i] = 0.0;

        for  ( j = 0 ; j < Noutputs ; j++ )  {
            tData->candCorr[i][j]        = 0.0;
            tData->candPrevCorr[i][j]    = 0.0;

            tData->candOut.weights[i][j] = random_weight( weightRange );
            tData->candOut.deltas[i][j]  = 0.0;
            tData->candOut.slopes[i][j]  = 0.0;
            tData->candOut.pSlopes[i][j] = 0.0;
        }
        for  ( j = 0 ; j < (Nunits+recurrent) ; j++ )  {
            tData->candIn.weights[i][j] = random_weight( weightRange );
            tData->candIn.deltas[i][j]  = 0.0;
            tData->candIn.slopes[i][j]  = 0.0;
            tData->candIn.pSlopes[i][j] = 0.0;
            if ( recurrent ) {
                tData->candDVdW[i][j]     = 0.0;
            }
        }
        if  ( candType == VARIED ) {
            switch ( i % 4 )  {
                case 0:  tData->candTypes[i] = SIGMOID;
                         break;
                case 1:  tData->candTypes[i] = ASIGMOID;
                         break;
                case 2:  tData->candTypes[i] = VARSIGMOID;
                         break;
                case 3:  tData->candTypes[i] = GAUSSIAN;
                         break;
            }
        } else {
            tData->candTypes[i] = candType;
        }
    }
}


/*  BUILD ERROR DATA -  Build a structure to store the error infromation on a
    network.
    */

error_data_t *build_error_data ( net_t *net )
{
    error_data_t *temp;
    int          Noutputs = net->Noutputs;
    char         *fn_name = "Build Network Error Data";

    temp =  (error_data_t *)alloc_mem ( 1, sizeof( error_data_t ), fn_name );
    temp->tempErrors = (float *)alloc_mem ( Noutputs, sizeof( float ), fn_name);
    temp->errors     = temp->tempErrors;
    temp->sumErr     = (float *)alloc_mem ( Noutputs, sizeof( float ), fn_name);

    return temp;
}


/*	FREE ERROR DATA -  Deallocate the structure used to keep track of
    error information.
    */

void free_error_data  ( error_data_t **data )
{
    free_mem( (*data)->tempErrors );
    free_mem( (*data)->sumErr );
    *data = free_mem( *data );
}


/*  INIT ERROR -  Initialize the values in an error structure to zero.
*/

void init_error ( error_data_t *error, int Noutputs )
{
    int i;

    error->bits       = 0;
    error->index      = 0.0;
    error->sumSqDiffs = 0.0;
    error->sumSqError = 0.0;
    for  ( i = 0 ; i < Noutputs ; i++ ) {
        error->sumErr[i] = 0.0;
    }
}
