# SURVIVOR_ant
A framework to annotate SVs (vcf file) with previous known SVs (vcf file) and or with genomic features (gff and or bed files).

**************************************

## INSTALL:

Download SURVIVOR:
```
git clone https://github.com/fritzsedlazeck/SURVIVOR_ant.git


cd SURVIVOR_ant
mkdir build
cd build
cmake ..
make
cd ../bin/survivor_ant-core-0.1.0
```

Note Mac users often have to provide parameters to the cmake command:
<pre>cmake -D CMAKE_C_COMPILER=/opt/local/bin/gcc-mp-4.7 -D CMAKE_CXX_COMPILER=/opt/local/bin/g++-mp-4.7 .. 
</pre>


**************************************

## Test data: 
Can be found here: 
https://github.com/NCBI-Hackathons/svcompare

**************************************

## USAGE: 

 ./survivor_ant --help

   ./survivor_ant  [-o <string>] [-l <int>] [-v <string>] [-g <string>] [-b
                   <string>] [--anno_distance <int>] [--svs_distance <int>]
                   -i <string> [--] [--version] [-h]


Where: 

   -o <string>,  --output <string>
      output file name

   -l <int>,  --min_length <int>
     Minimum length of SV to be reported. Default: 20

   -v <string>,  --vcf <string>
     VCF input file name(s)

   -g <string>,  --gff <string>
      gff annotation(s)

   -b <string>,  --bed <string>
      bed annotation(s)

   --anno_distance <int>
     Maximum distance to assign annotations to SVs. Default: 1000

   --svs_distance <int>
     Maximum distance to group SV together. Default: 1000

   -i <string>,  --svvcf <string>
     (required)  The vcf file that we should annotate

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.


**************************************
## CONTACT:

If you have questions or encounter a problem please contact:
fritz.sedlazeck@gmail.com
