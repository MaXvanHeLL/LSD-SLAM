# Modified LSD-SLAM project for my Augmented Reality Oculus Rift integration 

I haved used the Windows port of LSD-SLAM from here: https://github.com/williammc/lsd_slam and modified it to integrate it into my AR stereo engine for the Rift [1]

[1] https://github.com/MaXvanHeLL/ARift


### Related Papers

* **Augmented Reality Oculus Rift**, *M. Höll, N. Heran, V. Lepetit*, '16

* **LSD-SLAM: Large-Scale Direct Monocular SLAM**, *J. Engel, T. Schöps, D. Cremers*, ECCV '14

* **Semi-Dense Visual Odometry for a Monocular Camera**, *J. Engel, J. Sturm, D. Cremers*, ICCV '13


# How to build from source
The lsd slam code requires c++11 features.
Thus, it needs c++11 supported compiler to build the code from source.

## Windows (requiring Visual Studio 2013)
For windows build, please follow instructions from [Windows build](WindowsBuildInstruction.md)

# License
LSD-SLAM is licensed under the GNU General Public License Version 3 (GPLv3), see http://www.gnu.org/licenses/gpl.html.

For commercial purposes, the original lsd slam authors also offer a professional version under different licencing terms.
