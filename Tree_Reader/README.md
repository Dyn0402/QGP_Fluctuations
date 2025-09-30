# Quick Start

### Clone and Compile the Git Repositories

- Instructions for bash shell: `bash`
- Make a directory for your work and navigate to it
- Copy clone_and_compile.sh from Tree_Reader
- Make it executable with `chmod +x clone_and_compile.sh`
- Run it with `./clone_and_compile.sh`
- The repositories should now be cloned and compiled

### Run Single Energy as One Job on Condor

This TreeReder code was designed to run on a modern CPU utilizing multiple cores. 
The explanation below shows how to run on condor as a single job. 
A single condor process is exceedingly slow compared to modern CPUs, and the job will take a long time to complete.

**Important Note:** When running the 11.5 GeV data, the job took over two days to complete 
and the log file was only written once complete. If all energies need to be rerun, a parallelization framework should 
be developed.

- Go to the *subs* directory `cd QGP_Fluctuations/Tree_Reader/subs/`
- Edit the paths in `submit_single_job.xml` to point to your input and output directories.
- Edit the \<command> ./tree_reader arugments for the type of run:
  - The first argument is a beam energy (e.g. 7 &rarr; 7.7, 11 &rarr; 11.5, 19 &rarr; 19.6, 27, 39, 62 &rarr; 62.4)
  - The second argument is the *job type* (not important, just needs to contain `BES1_`)
  - The third argument is the *set group* (just a name for the output directory)
  - The fourth argument is the *set name* which specifies all the parameters of the analysis as a `_` separated string.
  - The fifth and sixth arguments are the start and number of times to run the set (for randomization sampling), e.g. `0 10` to run 10 times. Probably just leave as `0 0` for a single run.
  - An example command: `./tree_reader 11 BES1_Sim_flat80_simpleclust_s08a05_v207_res99_epbins1_test flat80_simpleclust_spread08_amp05_v207_res99_resample_epbins1_test Sim_flat80_simpleclust_spread08_amp05_v207_res99_norotate_resample_epbins1_ 0 0`
- Submit the job to the star manager `star-submit submit_single_job.xml`
- Wait for run to complete (days).


# To Compile:

- Clone and compile the ResearchLibs repository https://github.com/Dyn0402/Research_Libs
- Edit the Makefile to point to a ROOT6 installation in CXXFLAGS and LDFLAGS
- Edit the Makefile to point FLUCT_LIB_DIR to the FluctuationLib directory (within Research_Libs)
- Edit the /src/main.cpp file to run what you want
  - read_new() function
    - Jobs are organized by the "sets" vector. Each job is run independently and can be run in parallel.
    - Each job has a job type, which is used to determine which dataset to read and if it is a systematics job.
      - Each job has a number of set groups. A new output directory is created cor each set group.
        - Each set group can have multiple sets, whose names encode the parameters of the analysis.
        - In addition, each set has a pair of numbers defining how many times to run the set (useful for sampling randomization).
        - An example job:
            {"Sim_flat80_simpleclust_s08a05_v207_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp05_v207_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp05_v207_res99_norotate_resample_epbins1_", {0, 0}}}}}},
            The first string is the job name, which specifies the job is simulation (and a test).
            The second string is the set_group name, which will be the name of the output directory.
            The third string is the set name (the most important), which specifies the parameters of the analysis. In this example, a simulation is run with flat pdf of N-protons up to 80, using simple clustering with a gaussian width of 0.8 and amplitude of 0.5. Elliptic flow is also included with v2=0.07 and event plane resolution of 99%. The analysis is run with no rotation and resampling of the ep bins.
        - The energy_list vector is used to specify the energies for the analysis.

  - run_job() function
    - Edit the paths at the beginning to point to data files
    - Edit the divs vector to run the desired azimuthal partition bin widths
- Run `make` in the Tree_Reader directory
- Run the executable with `./tree_reader`


---

# Tree Reader More Generally – Compilation & Usage Instructions

## 📦 Dependencies

* Requires the [Research\_Libs](https://github.com/Dyn0402/Research_Libs) repository

---

## 🔧 Compilation Steps

1. **Clone and build Research\_Libs**

   ```bash
   git clone https://github.com/Dyn0402/Research_Libs.git
   cd Research_Libs/Fluctuation_Lib
   make
   ```
   
2. **Clone the QGP_Fluctuations repository**

   ```bash
   git clone https://github.com/Dyn0402/QGP_Fluctuations.git
    cd QGP_Fluctuations/Tree_Reader
    ```

3**Set the FluctuationLib directory in the Tree\_Reader Makefile**

   In `Tree_Reader/Makefile`, update the following variable:

   ```make
   FLUCT_LIB_DIR = /path/to/Research_Libs/Fluctuation_Lib
   ```

4**Edit the main function**

   In `Tree_Reader/src/main.cpp`, choose the entry point function you'd like to use, such as `read_new()` or `run_job()`.

---

## 🧪 Entry Point Details

### 🔁 `read_new()` Function

* Jobs are configured via the `sets` vector.
* Each job is executed independently and can be parallelized.
* Jobs contain:

  * A **job type**, used to determine dataset and whether it's a systematics job.
  * Multiple **set groups** — each group:

    * Creates a new output directory.
    * Contains multiple **sets**, each with:

      * A name that encodes analysis parameters.
      * A `{repeat_start, repeat_count}` pair for randomized sampling runs.

#### Example Job Structure

```cpp
{
  "Sim_flat80_simpleclust_s08a05_v207_res99_epbins1_test", {
    {
      "flat80_simpleclust_spread08_amp05_v207_res99_resample_epbins1_test", {
        {
          "Sim_flat80_simpleclust_spread08_amp05_v207_res99_norotate_resample_epbins1_", {0, 0}
        }
      }
    }
  }
}
```

* First string: job name (e.g., simulation test).

* Second string: set group name (becomes the output directory name).

* Third string: set name (defines analysis parameters like clustering, elliptic flow, and resolution).

* The `energy_list` vector sets which energies to run the analysis for.

---

### 🧭 `run_job()` Function

* Manually edit file paths near the top to point to the desired input data.
* Modify the `divs` vector to change azimuthal partition bin widths.

---

## 🛠️ Build the Project

From the `Tree_Reader` directory:

```bash
make
```

---

## 🚀 Run the Executable

```bash
./tree_reader
```

---
