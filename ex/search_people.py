import csv
import unicodedata

# ============================================
# CONFIG
# ============================================

CSV_FILE = "WATIQA_FULL.csv"
SEARCH_FILE = "search.txt"

# ============================================
# NORMALIZE TEXT
# ============================================

def normalize(text):
    """
    Normalize names:
    - lowercase
    - remove accents
    - remove extra spaces
    """

    if not text:
        return ""

    text = text.strip().lower()

    text = unicodedata.normalize("NFKD", text)
    text = "".join(c for c in text if not unicodedata.combining(c))

    text = " ".join(text.split())

    return text

def build_name_keys(full_name):
    """
    Create searchable versions of a name.

    Example:
        "Achraf Kouiss"

    produces:
        {
            "achraf kouiss",
            "kouiss achraf"
        }
    """

    full_name = normalize(full_name)

    parts = full_name.split()

    keys = set()

    if not parts:
        return keys

    # original
    keys.add(" ".join(parts))

    # reversed
    keys.add(" ".join(reversed(parts)))

    return keys

# ============================================
# LOAD SEARCH NAMES
# ============================================

search_names = set()

with open(SEARCH_FILE, "r", encoding="utf-8") as f:

    for line in f:

        line = line.strip()

        if not line:
            continue

        search_names.add(normalize(line))

# ============================================
# SEARCH CSV
# ============================================

found = {}
missing = set(search_names)

with open(CSV_FILE, "r", encoding="utf-8") as csvfile:

    reader = csv.DictReader(csvfile, delimiter=';', quotechar='"')

    for row in reader:

        # names to check
        fields_to_search = {
            "PERSON": row.get("NOM ET PRENOM", ""),
            "MOTHER": row.get("NOM ET PRENOM DE LA MERE", ""),
            "FATHER": row.get("NOM ET PRENOM DU PERE", "")
        }

        matched_any = False

        for relation, name_value in fields_to_search.items():

            keys = build_name_keys(name_value)

            matched = keys.intersection(search_names)

            if matched:

                matched_any = True

                for searched_name in matched:

                    if searched_name not in found:
                        found[searched_name] = []

                    found[searched_name].append({
                        "relation": relation,
                        "data": row
                    })

                    if searched_name in missing:
                        missing.remove(searched_name)

# ============================================
# PRINT RESULTS
# ============================================

print("\n================ FOUND ================\n")

for searched_name, matches in found.items():

    print(f"\n######## SEARCHED NAME: {searched_name} ########\n")

    for entry in matches:

        relation = entry["relation"]
        data = entry["data"]

        print(f"MATCH TYPE    : {relation}")
        print(f"NUM_COMMANDE  : {data['NUM_COMMANDE']}")
        print(f"NOM           : {data['NOM ET PRENOM']}")
        print(f"MERE          : {data['NOM ET PRENOM DE LA MERE']}")
        print(f"PERE          : {data['NOM ET PRENOM DU PERE']}")
        print(f"DATE NAISSANCE: {data['DATE DE NAISSANCE']}")
        print(f"EMAIL         : {data['EMAIL']}")
        print(f"TELEPHONE     : {data['TELEPHONE']}")
        print(f"ADRESSE       : {data['ADRESSE DE RESIDENCE']}")
        print(f"ACTE          : {data['NUMERO ACTE DE NAISSANCE']}")
        print(f"BUREAU        : {data['BUREAU ETAT CIVIL']}")
        print("-" * 60)

print("\n================ MISSING ================\n")

for name in sorted(missing):
    print(name)

    