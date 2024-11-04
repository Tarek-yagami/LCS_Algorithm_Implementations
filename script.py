with open("X_large.txt", "w") as f:
    f.write("ABABAB" * 1667)  # Generates approximately 10,002 characters

with open("Y_large.txt", "w") as f:
    f.write("BABABA" * 1667)  # Generates approximately 10,002 characters
