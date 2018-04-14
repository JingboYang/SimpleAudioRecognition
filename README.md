# SimpleAudioRecognition

This is a repository for University of Toronto Course ECE 532 Digital System Design ([An old course link here](http://www.eecg.toronto.edu/~pc/courses/532/2007/)). Official name of the project is `A Wirelessly Controlled Home Automation System`. It is a system built on [Nexys Video Board](https://store.digilentinc.com/nexys-video-artix-7-fpga-trainer-board-for-multimedia-applications/). This system runs a primitive audio recognition system and controls various [PMODs supplied by Digilent](https://store.digilentinc.com/pmod-modules/).

Here is a short video demonstration of our project.
[![Video Demonstration](https://img.youtube.com/vi/ksYiwKTFCrY/0.jpg)](https://www.youtube.com/watch?v=ksYiwKTFCrY)

## Contents

In this repository, you will find all components of our project. We have created tutorials for these projects as well. These tutorials describe how our project should be used and how to setup individual IPs. 

We used Xilinx WebPACK (download page [here](https://www.xilinx.com/support/download.html)). All of our Python code should work with both Python2 and Python3. You should run our Python code on Linux or Mac to accomodate hardcoded file path. 

 * Vivado project for our complete audio recognition hardware and software setup ([here](https://github.com/CaptainPenguins/SimpleAudioRecognition/tree/master/VivadoProject/Nexys-Video-DMA-BT-FFT-Full-Precision-Test))
    * Tutorial [here](https://github.com/CaptainPenguins/SimpleAudioRecognition/blob/master/doc/Floating_point_DMA_FFT_BT_Tutorial.pdf)
 * Vivado project for various PMOD contollers (Server, relay, ultrasonic and Bluetooth) ([here](https://github.com/CaptainPenguins/SimpleAudioRecognition/tree/master/VivadoProject/Nexys-Video-Hub))
    * Tutorial [here](https://github.com/CaptainPenguins/SimpleAudioRecognition/blob/master/doc/Hub_Board_PMOD_Tutorial.pdf)
 * Python project for a simple audio matching algorithm ([here](https://github.com/CaptainPenguins/SimpleAudioRecognition/tree/master/PythonProject))
    * Tutorial [here](https://github.com/CaptainPenguins/SimpleAudioRecognition/blob/master/PythonProject/JupyterNotebook/Instruction.ipynb)
 * C translation of the Python project managed by Visual Studio ([here](https://github.com/CaptainPenguins/SimpleAudioRecognition/tree/master/VisualStudioProject))
 * Other Python utilities that we have used ([here](https://github.com/CaptainPenguins/SimpleAudioRecognition/tree/master/MicroblazePlotTools))

 [DesignTree](https://github.com/CaptainPenguins/SimpleAudioRecognition/blob/master/DesignTree.md) is a document outlining all of our design components. You can also find a [formal report](https://github.com/CaptainPenguins/SimpleAudioRecognition/blob/master/doc/FinalReport.pdf) for the project.


## Contributors

We are Jingbo, Peter and Wenxuan from Division of Engineering Science at University of Toronto. 

Naif was TA for our project and [Professor Paul Chow](http://www.eecg.toronto.edu/~pc/) was our instructor.


## Acknowledgments

Our PMOD modules rely on many Digilent example projects. A generic tutorial of Digilent's PMOD can be found [here](https://reference.digilentinc.com/learn/programmable-logic/tutorials/pmod-ips/start). 

Our audio recognition system is based on research works, an example of which is [Voice Recognition Algorithms using Mel Frequency Cepstral Coefficient (MFCC) and Dynamic Time Warping (DTW) Techniques](https://arxiv.org/ftp/arxiv/papers/1003/1003.4083.pdf). 


## Disclaimar
Digilent libraries do not come with any license. We will add license if Digilent chooses to use distribution license in the future.

We do not intend to use a license for our project either. Contents that we have created, including source code, tutorial and documentation, shoud be treated as is. We do not assume responsibility or liability for the use or distribution of contents of this repository.
