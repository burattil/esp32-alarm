import csv
import os
from gtts import gTTS

# Create the folders to store the words if they do not exist
os.makedirs("audio/english", exist_ok=True)
os.makedirs("audio/italian", exist_ok=True)

# Use the supplied Words.csv to create the mp3 files
with open("Words.csv", newline="", encoding="utf-8") as file:
    # Use the dictionary tool to separate the columns in the csv
    reader = csv.DictReader(file)
    
    # Go over each row in the "separated" file
    for row in reader:
        # Create the variables for each "component"
        wordID = int(row["ID"])
        englishWord = row["English"].strip()
        italianWord = row["Italian"].strip()
        
        # Create the English and Italian files with naming convention ID_word.mp3
        englishFile = f"audio/english/{wordID:03d}_{englishWord}.mp3"
        italianFile = f"audio/italian/{wordID:03d}_{italianWord}.mp3"
        
        # Skip the files that already exist to avoid duplicates
        if not os.path.exists(englishFile):
            # Generate the English mp3 file
            gTTS(text=englishWord, lang="en").save(englishFile)
            
        # Skip the files that already exist to avoid duplicates
        if not os.path.exists(italianFile):
            # Generate the Italian mp3 file
            gTTS(text=italianWord, lang="it").save(italianFile)
            
print("Done!")