#!/bin/bash

# Clone and compile the Research_Libs and QGP_Fluctuations repositories
# This script is in the QGP_Fluctuations/Tree_Reader directory, maybe a bit circular...

# Exit on any error
set -e

# Source dylan's root6
source /star/u/dneff/Software/root/root-6.14.06/obj/bin/thisroot.sh
alias root='root -l'

# Clone the repositories
git clone https://github.com/Dyn0402/Research_Libs.git
git clone https://github.com/Dyn0402/QGP_Fluctuations.git

# Build Fluctuation_Lib
cd Research_Libs/Fluctuation_Lib
make

# Build Tree_Reader
cd ../../QGP_Fluctuations/Tree_Reader
make


