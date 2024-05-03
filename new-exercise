#!/usr/bin/env python3

import os


def replace_placeholder(filename: str, placeholder: str, new_value: str):
    with open(filename, "r") as f:
        lines = f.readlines()

    with open(filename, "w") as f:
        for line in lines:
            if placeholder in line:
                line = line.replace(placeholder, new_value)
                f.write(f"\t\"{new_value}\"\n")
                f.write(f"{placeholder}\n")
            else:
                f.write(line)


def main():
    # Get the exercise name from the user
    exercise_name: str = input("Exercise name: ")

    # Ask if he wants to add vertex and fragment shaders (default: yes)
    add_shaders: bool = False if input("Add shaders? [Y/n]: ") == "n" else True

    # Create the exercise directory
    PROJECT_SRC_DIR = os.path.join(os.getcwd(), "src")
    exercise_dir = os.path.join(PROJECT_SRC_DIR, exercise_name)

    if os.path.exists(exercise_dir):
        print(f"ERROR! Exercise {exercise_name} already exists")
        exit(1)

    os.makedirs(exercise_dir, exist_ok=True)

    # Create an empty C++ source file
    open(os.path.join(exercise_dir, f"{exercise_name}.cpp"), "w")

    # Create shaders if needed
    if add_shaders:
        open(os.path.join(exercise_dir, f"{exercise_name}.vs"), "w")
        open(os.path.join(exercise_dir, f"{exercise_name}.fs"), "w")

    # Update CMakelists.txt
    replace_placeholder("CMakeLists.txt",
                        "#-#-#PYTHON-NEW-EXERCISE-PLACEHOLDER-#-#-#",
                        exercise_name)
    
    # Print success message
    print(f"Exercise {exercise_name} created successfully!")
    print("Your CMakeLists.txt has been updated.")

if __name__ == "__main__":
    main()