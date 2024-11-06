#!/bin/bash

# Diretório onde os arquivos serão armazenados
OUTPUT_DIR="genome_data"
PROCESSED_DIR="processed_data"
mkdir -p $OUTPUT_DIR
mkdir -p $PROCESSED_DIR

# Loop para baixar, descompactar e preprocessar cada cromossomo
for i in {1..22}; do
    FILE="chr${i}.subst.fa"
    URL="ftp://hgdownload.cse.ucsc.edu/goldenPath/hg19/snp147Mask/${FILE}.gz"
    
    # Baixar o arquivo
    echo "Baixando $FILE..."
    wget -q -P $OUTPUT_DIR $URL

    # Descompactar o arquivo
    echo "Descompactando $FILE.gz..."
    gunzip "$OUTPUT_DIR/${FILE}.gz"
    
    # Pré-processamento: manter apenas A, T, C, G, converter para maiúsculas, e remover espaços extras
    echo "Pré-processando $FILE..."
    awk '/^>/ {print; next} {gsub(/[^ATCG]/, ""); print toupper($0)}' "$OUTPUT_DIR/$FILE" > "$PROCESSED_DIR/$FILE"
done

echo "Downloads, descompactação e pré-processamento concluídos!"
