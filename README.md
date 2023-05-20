# Language Detection using Copy Models

## Lang

Runs the copy model for a certain target using a reference file.

### How to run?

#### Parameters

- `-r`: Reference file path
- `-i`: Target file path
- `-o`: Output file path
- `-m`: Output model path
- `-a`: Alpha (Default: 0.5)
- `-k`: Window size (Default: 3)
- `-t`: Threshold (Default: 0.4)
- `-h`: Help

#### Example

```bash
lang -r ../references/europarl20/portuguese.txt
-m ../models
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
- `-m`: Output model path (Optional)

#### Example

```bash
python3 findlang.py -e ./cmake-build-debug/lang -r ../references/europarl20/ -t ../references/target/single_language/ -it 
```