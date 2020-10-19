 /********************************************************************************************
 *
 *  FastK: a rapid disk-based k-mer counter for high-fidelity shotgun data sets.
 *     Uses a novel minimizer-based distribution scheme that permits problems of
 *     arbitrary size, and a two-staged "super-mer then weighted k-mer" sort to acheive
 *     greater speed when error rates are low (1% or less).  Directly produces read
 *     profiles.
 *
 *  Author:  Gene Myers
 *  Date  :  October, 2020
 *
 *********************************************************************************************/

#ifndef _KMERS
#define _KMERS

#define DEVELOPER

#define IO_BUF_LEN   4096       // number of uint's in bit stuffed IO buffer for each part+thread
#define IO_UBITS       64
#define IO_UBYTES       8
#define IO_UTYPE   uint64

#define NPANELS 4

#ifdef DEVELOPER

extern int DO_STAGE;  // Stage to run if code development (not for users)

#endif

  //  Option Settings

extern int    VERBOSE;     //  show progress
extern int64  SORT_MEMORY; // Memory available for each k-mer sort 
extern int    KMER;        //  desired K-mer length
extern int    NTHREADS;    //  # of threads to run with
extern char  *SORT_PATH;   //  where to put external files

extern int    HIST_LOW;    // Zero or start count for histogram
extern int       HIST_HGH; // End count for histogram
extern int    DO_TABLE;    // Zero or table cutoff
extern int    DO_PROFILE;  // Do or not

  //  Sizes and numbers of items (k-mers, super-mers, reads, positions)

extern int    NPARTS;      //  number of k-mer buckets
extern int    SMER;        //  max size of a super-mer (= MAX_SUPER + KMER - 1)ZZ
extern int64  KMAX;        //  max k-mers in any part
extern int64  NMAX;        //  max super-mers in any part

extern int    MOD_LEN;     //  length of minimizer buffer (power of 2)
extern int    MOD_MSK;     //  mask for minimzer buffer

extern int    MAX_SUPER;   //  = EMER - MIN_L1  (where EMER = KMER - 2*PAD)

extern int    SLEN_BITS;      //  # of bits needed to encode the length of a super-mer (less KMER-1)
extern uint64 SLEN_BIT_MASK;  //  Bit-mask for super-mer lengths

extern int RUN_BITS;     //  # of bits encoding largest run index
extern int RUN_BYTES;    //  # of bytes encoding largest run index

extern int SLEN_BYTES;   //  # of bytes encoding Length of a super-mer
extern int PLEN_BYTES;   //  # of bytes encoding length of a compressed profile segment
extern int PROF_BYTES;   //  # of bytes encoding RUN+PLEN_BYTES

extern int KMER_BYTES;   //  # of bytes encoding a KMER 
extern int SMER_BYTES;   //  # of bytes encoding a super-mer 
extern int KMAX_BYTES;   //  # of bytes to encode an int in [0,KMAX)

extern int KMER_WORD;    //  # of bytes in a k-mer entry
extern int SMER_WORD;    //  # of bytes in a super-mer entry
extern int TMER_WORD;    //  bytes to hold a k-mer/count table entry
extern int CMER_WORD;    //  bytes to hold a count/position entry

extern int NUM_READS;    //  Number of reads in data set

extern uint8 Comp[256];  //  complement of 4bp byte code

  //  IO Module Interface

typedef struct
  { int         maxlen;   //  length of maximum read in dataset
    int64       totlen;   //  total # of bases in data set
    int         nreads;   //  # of reads in data set
    double      ratio;    //  ratio of file size to portion read (first block only)
    int64       maxbps;   //  size of bases array
    int         maxrds;   //  size of boff array
    char       *bases;    //  concatenation of 0-terminated read strings
    int64      *boff;     //  read i is at bases+boff[i], boff[n] = total bytes
  } DATA_BLOCK;

typedef void *Input_Partition;

  Input_Partition *Partition_Input(int argc, char *argv[]);

  DATA_BLOCK *Get_First_Block(Input_Partition *part, int64 numbps);

  void Free_First_Block(DATA_BLOCK *block);

  char *First_Root_Name(Input_Partition *part);

  void Scan_All_Input(Input_Partition *part);

  void Free_Input_Partition(Input_Partition *part);

  //  Stages

int Determine_Scheme(DATA_BLOCK *block);
 
void Split_Kmers(Input_Partition *io, char *root);

  void Distribute_Block(DATA_BLOCK *block, int tid);

void Sorting(char *dpwd, char *dbrt);

void Merge_Tables(char *dpwd, char *dbrt);

void Merge_Profiles(char *dpwd, char *dbrt);

  //  Sorts

typedef struct
  { int    beg;     //  [beg,end) first byte with sorted data at off
    int    end;
    int64  off;
    int64  khist[256];
    int64  count[0x8000];
    int    byte1;   //  used internallly by sort
  } Range;

void Supermer_Sort(uint8 *array, int64 nelem, int rsize, int ksize,
                   int64 *part, int nthreads, Range *panels);

void Weighted_Kmer_Sort(uint8 *array, int64 nelem, int rsize, int ksize,
                        int64 *part, int nthreads, Range *panels);

void MSD_Sort(uint8 *array, int64 nelem, int rsize, int ksize,
              int64 *part, int nthreads, Range *panels);

void *LSD_Sort(int64 nelem, void *src, void *trg, int rsize, int *bytes);

#endif // _KMERS