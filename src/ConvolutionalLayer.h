// Copyright Hugh Perkins 2014 hughperkins at gmail
//
// This Source Code Form is subject to the terms of the Mozilla Public License, 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "Layer.h"
#include "OpenCLHelper.h"
//#include "ClConvolve2.h"
#include "ActivationFunction.h"
#include "LayerMaker.h"
#include "Timer.h"
#include "StatefulTimer.h"
#include "stringhelper.h"
#include "LayerDimensions.h"

#define VIRTUAL virtual

class Forward;
class Backward;
class BackpropWeights;
class ConvolutionalMaker;

class ConvolutionalLayer : public Layer {
public:
    OpenCLHelper *const cl; // NOT owned by us
    Trainer *weightsTrainer; // OWNED by us, we should delete (if non-zero)
    Trainer *biasWeightsTrainer; // OWNED by us, we should delete (if non-zero)

    Forward *forwardimpl;
    BackpropWeights *backpropWeightsImpl;
    Backward *backwardImpl;

    LayerDimensions dim;
//    ActivationFunction const *const activationFunction;

    float *weights;
    float *biasWeights;
    float *output;
    float *gradInput;
    float *gradWeights;
    float *gradBiasWeights;

//    const int filterSize;
//    const int filterSizeSquared;
//    const bool padZeros;

    CLWrapper *weightsWrapper;
    CLWrapper *outputWrapper;
    CLWrapper *gradInputWrapper;
    CLWrapper *gradWeightsWrapper;
    CLWrapper *gradBiasWeightsWrapper;

    int batchSize;
    int allocatedSpaceNumExamples;

    bool weightsCopiedToHost;
    bool outputCopiedToHost;
    bool gradInputCopiedToHost;
    bool gradWeightsCopiedToHost;
    bool gradBiasWeightsCopiedToHost;

    inline int getWeightIndex( int filterId, int inputPlane, int filterRow, int filterCol ) const {
        return ( ( filterId 
            * dim.inputPlanes + inputPlane )
            * dim.filterSize + filterRow )
            * dim.filterSize + filterCol;
    }
    inline float getWeight( int filterId, int inputPlane, int filterRow, int filterCol ) const {
//        getWeights();
        return weights[ getWeightIndex( filterId, inputPlane, filterRow, filterCol ) ];
    }
    inline int getOutputIndex( int n, int outPlane, int outRow, int outCol ) const {
        return ( ( n
            * dim.numFilters + outPlane )
            * dim.outputImageSize + outRow )
            * dim.outputImageSize + outCol;
    }
    inline float getOutput( int n, int outPlane, int outRow, int outCol ) const {
        return output[ getOutputIndex(n,outPlane, outRow, outCol ) ];
    }

//    ConvolutionalLayer( Layer *previousLayer, ConvolutionalMaker const*maker );
    // images are organized like [imageId][plane][imagerow][imagecol]
    // filters are organized like [filterid][plane][filterrow][filtercol]
    // output are organized like [imageid][filterid][imagerow][imagecol]
//    inline int getWeightIndex( int outPlane, int inPlane, int filterrow, int filtercol ) const {
//        return ( ( outPlane * upstreamNumPlanes 
//             + inPlane ) * filterSize 
//             + filterrow ) * filterSize
//             + filtercol;
//    }
//    inline float getWeight( int outPlane, int inPlane, int filterrow, int filtercol ) const {
//        return weights[getWeightIndex( outPlane, inPlane, filterrow, filtercol ) ];
//    }

    // [[[cog
    // import cog_addheaders
    // cog_addheaders.add()
    // ]]]
    // generated, using cog:
    ConvolutionalLayer( OpenCLHelper *cl, Layer *previousLayer, ConvolutionalMaker *maker );
    VIRTUAL ~ConvolutionalLayer();
    VIRTUAL std::string getClassName() const;
    VIRTUAL float *getGradInput();
    VIRTUAL float *getGradWeights();
    VIRTUAL float *getGradBiasWeights();
    VIRTUAL bool providesGradInputWrapper() const;
    VIRTUAL CLWrapper *getGradInputWrapper();
    VIRTUAL bool hasOutputWrapper() const;
    VIRTUAL CLWrapper *getOutputWrapper();
    VIRTUAL bool needsBackProp();
    VIRTUAL float const *getWeights() const;
    VIRTUAL float *getWeights();
    VIRTUAL float *getBiasWeights();
    VIRTUAL int getOutputSize() const;
    VIRTUAL int getOutputPlanes() const;
    VIRTUAL int getOutputImageSize() const;
    void randomizeWeights();
    VIRTUAL void print();
    VIRTUAL void printWeights();
    VIRTUAL void printOutput();
    VIRTUAL void setBatchSize( int batchSize );
    VIRTUAL void forward();
    VIRTUAL float * getOutput();
    VIRTUAL void setWeights( float *weights, float *biasWeights );
    VIRTUAL void initWeights( float const*weights );
    VIRTUAL int getOutputCubeSize() const;
    VIRTUAL int getPersistSize() const;
    VIRTUAL void persistToArray(float *array);
    VIRTUAL void unpersistFromArray(float const*array);
    VIRTUAL void initBiasWeights( float const*biasWeights );
    VIRTUAL int getWeightsSize() const;
    VIRTUAL int getBiasWeightsSize() const;
    VIRTUAL void backward( float learningRate );
    VIRTUAL std::string asString() const;
    VIRTUAL bool needsTrainer() const;
    VIRTUAL void setTrainerMaker( TrainerMaker *trainerMaker );

    // [[[end]]]
};

