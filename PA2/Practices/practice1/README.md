# Practice 1

## COVID-19 Phone Book Reporter

Your task is to implement a program that simplifies searching a phone book, as described below.

### Required function

```cpp
bool report(const std::string& fileName, std::ostream& out);
```

- `fileName` is the name of the phone book file to analyze.
- `out` is the output stream where the report must be written.
- Return `true` if all processing was OK, or `false` if there was an error in the file or its format.

### Input file format

The input file contains:
1. A list of contacts.
2. An empty separator line.
3. A list of searches.

Each contact is on a single line and contains:
- name
- surname
- phone number

Fields are separated by one or more white-space characters. Newline characters are only used to separate lines, not fields.

The search section contains one or more words. Each search word is a name or surname to look for.

### Matching rules

- A record matches if the search string matches the name, the surname, or both.
- Matching must be exact: `Joh` does not match `John`.
- Preserve the original input order of matched records.
- Write all output only to the provided `out` stream.
- Do not print anything to `std::cout` or `std::cerr`.

### Output rules

- The output format must follow the attached examples.
- Every output line must end with a newline (`\n`).
- Include the newline after the last output line as well.

### Format errors

Return `false` if any of the following is found:
- A field is missing.
- A phone number is not exactly 9 digits long.
- A phone number contains a non-digit character.
- A phone number starts with `0`.
- There are extra non-whitespace characters after the phone number on a contact line.
- The empty line separator between contacts and searches is missing.

### Notes

- Example input files are attached in the archive.
- You can use `std::vector`, `std::array`, and C++ standard algorithms, but they are not required.
- The problem can be solved with basic arrays if you prefer.
- `std::getline` and `std::stringstream` may be useful.
- Do not use `std::ws`.
- The task focuses on file processing, so performance is not the main concern.

### Implementation

Create your implementation file inside this folder, for example:
- `report.cpp`
- or `practice1.cpp`

Put your `report` function implementation there and test it with sample input files.
