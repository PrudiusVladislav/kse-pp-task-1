# Gap Buffer Test Workflows

This document describes several workflows for testing the text editor implementation.

# Navigation
- Task 1 (procedural style text editor in C)
  - [Workflow 1](#workflow-1)
  - [Workflow 2](#workflow-2)
  - [Workflow 3](#workflow-3)
  - [Workflow 4](#workflow-4)
  - [Workflow 5](#workflow-5)

- Task 2 (OOP style text editor in C++)
  - [Workflow 6](#workflow-6)
  - [Workflow 7](#workflow-7)
  - [Workflow 8](#workflow-8)
  - [Workflow 9](#workflow-9)
  - [Workflow 10](#workflow-10)

- Task 4 (work with dll, for encrypting/decrypting files)
  - [Workflow 11](#workflow-11)
  - [Workflow 12](#workflow-12)

## Workflow 1

1. Use the `append` command to append text to the end of the buffer.
2. Use the `print` command to print the contents of the buffer.

![Workflow 1 Screenshot](/workflows/images/task1/workflow1_result.png)

## Workflow 2

1. Use the `append` command to append text to the end of the buffer.
2. Use the `nl` command to start a new line.
3. Use the `append` command to append more text.
4. Use the `print` command to print the contents of the buffer.

![Workflow 2 Screenshot](/workflows/images/task1/workflow2_result.png)

## Workflow 3

1. Use the `append` command to append text to the end of the buffer.
2. Use the `save` command to save the buffer to a file.
3. Use the `print` command to print the contents of the buffer.
4. Use the `load` command to load the previously saved file into the buffer.
5. Use the `print` command to print the contents of the buffer.

![Workflow 3 Screenshot](/workflows/images/task1/workflow3_result.png)

## Workflow 4

1. Use the `append` command to append text to the end of the buffer.
2. Use the `insert` command to insert text at a specific line and index.
3. Use the `print` command to print the contents of the buffer.
4. Use the `save` command to save the buffer to a file.
5. Use the `load` command to load the previously saved file into the buffer.
6. Use the `print` command to print the contents of the buffer.

![Workflow 4 Screenshot](/workflows/images/task1/workflow4_result.png)

## Workflow 5

1. Use the `append` command to append text to the end of the buffer.
2. Use the `search` command to search for text in the buffer.

![Workflow 5 Screenshot](/workflows/images/task1/workflow5_result.png)

## Workflow 6 (start of the assignment 2 (OOP hell))

1. Use the `append` command to append text to the end of the buffer.
2. Use the `delete` command to delete a portion of the text.
3. Use the `print` command to print the contents of the buffer.

![Workflow 6 Screenshot](/workflows/images/task2/workflow6_result.png)

## Workflow 7

1. Use the `append` command to append text to the end of the buffer.
2. Use the `cut` command to cut a portion of the text.
3. Use the `print` command to print the contents of the buffer.
4. Use the `paste` command to paste the previously cut text.
5. Use the `print` command to print the contents of the buffer.

![Workflow 7 Screenshot](/workflows/images/task2/workflow7_result.png)

## Workflow 8

1. Use the `append` command to append text to the end of the buffer.
2. Use the `copy` command to copy a portion of the text.
3. Use the `print` command to print the contents of the buffer.
4. Use the `paste` command to paste the previously copied text.
5. Use the `print` command to print the contents of the buffer.

![Workflow 8 Screenshot](/workflows/images/task2/workflow8_result.png)

## Workflow 9

1. Use the `append` command to append text to the end of the buffer.
2. Use the `insertWithReplace` command to insert new text and replace a portion of the existing text.
3. Use the `print` command to print the contents of the buffer.

![Workflow 9 Screenshot](/workflows/images/task2/workflow9_result.png)

## Workflow 10

1. Use the `append` command to append text to the end of the buffer.
2. Use the `cut` command to cut a portion of the text.
3. Use the `print` command to print the contents of the buffer.
4. Use the `paste` command to paste the previously cut text.
5. Use the `insertWithReplace` command to insert new text and replace a portion of the existing text.
6. Use the `print` command to print the contents of the buffer.

![Workflow 10 Screenshot](/workflows/images/task2/workflow10_result.png)

## Workflow 11

1. The original content of the file is:

![Original File](/workflows/images/task4/original.png)

2. Use the `encrypt` command to encrypt a file.
![Encrypt File](/workflows/images/task4/workflow_11.png)

3. Encrypted file:

![Encrypted File](/workflows/images/task4/encrypted.png)

## Workflow 12

1. Use the `decrypt` command to decrypt the previously encrypted file.
![Decrypt File](/workflows/images/task4/workflow_12.png)

