# Language Detection using Copy Models

## Lang

Runs the copy model for a certain target using a reference file.

**Note**: The Lang model **will not** create the output directory if it doesn't exists. Please make sure to pass a path for a directory that exists in the `-o` parameter.

### How to run?

#### Parameters

- `-r`: Reference file path
- `-i`: Target file path
- `-o`: Output file path
- `-a`: Alpha (Default: 0.5)
- `-k`: Window size (Default: 3)
- `-t`: Threshold (Default: 0.4)
- `-h`: Help

#### Example

```bash
lang -r ../references/europarl20/portuguese.txt
-i ../references/target/dutch_hungarian_portuguese.txt
-o ../output/test.txt
```

## Find Lang

Allows to discover the language for a target text using the amount of information provided by the lang (Copy Model).

### How to run?

#### Parameters

- `-e`: Executable path
- `-r`: Reference file path
- `-it`: Interactive mode
- `-t`: Target file path
- `-o`: Output file path (Optional)
- `-k`: Window Size for Lang CPM
- `-nFC`: Disable Finite-Context for Lang CPM

#### Example

```bash
python3 findlang.py -e ./cmake-build-debug/lang -r ../references/europarl20/ -t ../references/target/single_language/ -it 
```

## Locate Lang

Allows to identify languages in a text file with mixed languages. It also indicates the position where those langues were identified.

### How to run?

#### Parameters

- `-e`: Executable path
- `-r`: Directory with reference files
- `-t`: Target file
- `-it`: Interactive mode
- `-w`: Window size
- `-O`: Optimization Level
- `-o`: Window Overlap
- `-k`: Window Size for Lang CPM
- `-nFC`: Disable Finite Context for Lang CPM

#### Example 

```bash
python3 locatelang.py -e ../bin/lang -r ../references/europarl20/ -t ../target/french_romanian_greek.txt
```