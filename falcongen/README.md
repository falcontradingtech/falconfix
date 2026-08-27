# FalconGen Usage Guide

This guide explains how to use the FalconGen tools to generate code, prepare configuration files, and build/run benchmarks and roundtrip tests.

## 1. Generating Output and Code

FalconGen is designed to generate C++ code, test files, and protocol definitions from XML and YAML sources. The main entry point is the Python script:

```
falcongen/falcon-gen.py
```

### Typical Usage

From the project root or the `falcongen/` directory, run:

```
python3 falcon-gen.py
```

This will process the protocol XML and YAML files and generate output in:
- `falcongen/output/cpp/` (C++ source code)
- `falcongen/output/include/` (C++ headers)
- `falcongen/output/tests/` (test files)

## 2. Preparing Configuration Files

Before code generation, make sure the configuration files are present in:

```
falcongen/config/
```


The most important files are:
- `falcongen/config/samples.yaml` (can be generated via command or auto-generated if missing)
- `falcongen/config/string_sizes.yaml` (can be generated via command)

### Generating string_sizes.yaml
To generate a template for string_sizes.yaml based on your protocol XML, run:

```
python3 falcon-gen.py --string-size-template --xml-file=templates/FIX44.xml
```

This will create or update `falcongen/config/string_sizes.yaml`.

### Generating samples.yaml
To generate a template for samples.yaml based on your protocol XML, run:

```
python3 falcon-gen.py --samples-template --xml-file=templates/FIX44.xml
```

This will create or update `falcongen/config/samples.yaml`. You can also edit it manually to customize sample messages for tests and benchmarks. If missing, it will be auto-generated during code generation.

> Tip: If you change any YAML or XML in `falcongen/config/`, always re-run the code generation step before building.

#### About the configuration files

- **falcongen/config/samples.yaml** — This file defines the sample values and overrides that the generator will use to create example FIX messages for all message types defined in your protocol XML (e.g., templates/FIX44.xml). You can customize default values, required fields, and specific overrides for test and benchmark generation.

- **falcongen/config/string_sizes.yaml** — This file lists the maximum sizes for all string-type fields in the protocol version defined by your XML template. It is used to ensure that generated code and tests respect the correct field length constraints for each string field in the FIX dictionary.

## 3. Building and Running Benchmarks

To build and run the FalconGen benchmarks, use the provided script:

```
./falcongen/build_and_run_bench.sh
```

This script will:
- Build the benchmark targets (using CMake and Ninja)
- Run the generated benchmarks
- Output results to the console and relevant log files

## 4. Building and Running Roundtrip Tests

To build and run the roundtrip protocol tests, use:

```
./falcongen/build_and_run_roundtrip_tests.sh
```

This script will:
- Build the roundtrip test targets
- Execute the tests
- Print a summary of the results

## Notes
- Make sure you have all prerequisites installed (Python 3, CMake, Ninja, C++ compiler, etc).
- If you modify any protocol XML or YAML in `falcongen/config/`, always re-run `falcon-gen.py` before building.
- For advanced usage or troubleshooting, check the comments in each script or the main project README.
