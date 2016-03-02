# Optical Flow
#### C++ (_experimental_) implementation of methods for estimating optical flow

Optical flow is the pattern of apparent motion of objects in a visual scene caused by the relative motion between an observer (an eye or a camera) and the scene. ([Wikipedia](https://en.wikipedia.org/wiki/Optical_flow/)).

Methods:
* Horn & Schunck
* Lucas & Kanade
* Lucas & Kanade Pyramidal

#### Result Example

![Result example](data/lkc2f/flow.png)

### References
* [B.K.P. Horn and B.G. Schunck, "Determining optical flow." Artificial Intelligence, vol 17, pp 185–203, 1981](http://dspace.mit.edu/handle/1721.1/6337)
* [B. D. Lucas and T. Kanade, "An iterative image registration technique with an application to stereo vision". Proceedings of Imaging Understanding Workshop, pages 121--130, 1981]( http://www-cse.ucsd.edu/classes/sp02/cse252/lucaskanade81.pdf)
* [Bouguet, J.-Y., "Pyramidal Implementation of the Lucas Kanade Feature Tracker Description of the algorithm". Intel Corporation Microprocessor Research Labs, 2000](http://robots.stanford.edu/cs223b04/algo_tracking.pdf)
