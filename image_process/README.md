##Requirements:
   * python 3.5.4
   * jupyter 1.0.0
   * matplotlib 2.0.2
   * numpy 1.12.1

## Use pixel desktop.
```shell
source /home/pixel/miniconda3/bin/activate
source activate py3.5
jupyter notebook
```

## Install from scratch
I suggest to use a conda environment, but it requires additional work.
First download and install a [Miniconda](https://conda.io/miniconda.html).
Then create a environment by running:
```unix
conda env create -f environment.yml
```

then you need to install openCV again for this type of python.
Here is an example I used in *macOS*. Change paths, as needed.
```unix
cd <open_cv-repository>
mkdir build_for_py3.5; cd build_for_py3.5
cmake -DCMAKE_BUILD_TYPE=Release -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules -DPYTHON3_PACKAGES_PATH=${HOME}/miniconda3/envs/py3.5/lib/python3.5/site-packages  -DPYTHON3_LIBRARY=${HOME}miniconda3/envs/py3.5/lib/libpython3.5m.dylib -DPYTHON3_INCLUDE_DIR=${HOME}/miniconda3/envs/py3.5/include/python3.5m ..

make -j8
sudo make install
```
To unstall it: `sudo make uninstall`

then you can invoke jupyter
```unix
jupyter notebook
```
