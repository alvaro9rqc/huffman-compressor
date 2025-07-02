#include "io_tool.h"
#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096 // can be parameterized

/*
 * 1 if is internal node, 0 if leaf
 * there is a byte after the indicator
 * always return new position in buffer
 */
int io_write_in_orden(Node *node, unsigned char *buffer, int index) {
  if (node->is_leaf) {
    buffer[index++] = 0; // Indicate leaf
    buffer[index++] = node->byte;
  } else {
    buffer[index++] = 1; // Indicate internal node
    index = io_write_in_orden(node->left, buffer, index);
    index = io_write_in_orden(node->right, buffer, index);
  }
  return index;
}

/*
 * Use BUFFER_SIZE to write the huffman tree
 */
int io_write_huffman_tree(FILE *wfile, Node *root) {
  int status = 0;
  if (root == NULL)
    return -1;
  // Write the tree in-order
  // Precondition: each node has two children or is a leaf
  unsigned char buffer[BUFFER_SIZE];
  int size = io_write_in_orden(root, buffer, 0);
  fwrite(buffer, sizeof(unsigned char), size, wfile);
  return status;
}

/*
 * NOTE: I am using 'long long' to save the file size, take care with capacity
 */
[[nodiscard]]
int io_write_huffman_code(FILE *wfile, unsigned char **huff_code,
                          char *file_name) {
  FILE *rfile = fopen(file_name, "rb");
  if (rfile == NULL) {
    fprintf(stderr, "No se pudo abrir el archivo: %s\n", file_name);
    return -1;
  }
  // save file size
  fseek(rfile, 0, SEEK_END);
  long long file_size = ftell(rfile);
  fseek(rfile, 0, SEEK_SET);
  // Write file size
  if (fwrite(&file_size, sizeof(long long), 1, wfile) < 1) {
    fprintf(stderr, "Error writing file size to file.\n");
    fclose(rfile);
    return -1;
  }
  unsigned char rbuff[BUFFER_SIZE];
  size_t r_s = 0; // read bytes
  unsigned char wbuff[BUFFER_SIZE];
  size_t w_lim = BUFFER_SIZE * 8;
  size_t w_idx = 0; // new position in write buffer
  while ((r_s = fread(rbuff, 1, BUFFER_SIZE, rfile)) > 0) {
    for (size_t i = 0; i < r_s; ++i) {
      unsigned char c = rbuff[i];
      // if has space
      if (huff_code[c][0] <= w_lim - w_idx) {
        for (size_t j = 0; j < huff_code[c][0]; ++j) {
          // locate byte and write bit
          wbuff[w_idx / 8] |= 1 << (7 - (w_idx % 8));
          ++w_idx;
        }
      } else {
        // write buffer
        size_t w_size = w_idx / 8;
        if (fwrite(wbuff, sizeof(unsigned char), w_size, wfile) < w_size) {
          fprintf(stderr, "Error writing huffman code to file.\n");
          fclose(rfile);
          return -1;
        }
        // save the rest of the bits
        if (w_idx % 8 != 0) {
          wbuff[0] = w_idx / 8;
        }
        w_idx %= 8; // reset index to write
        // write last char
        for (size_t j = 0; j < huff_code[c][0]; ++j) {
          // locate byte and write bit
          wbuff[w_idx / 8] |= 1 << (7 - (w_idx % 8));
          ++w_idx;
        }
      }
    }
  }
  // Write the remaining bits in the buffer
  if (w_idx > 0) {
    size_t w_size = w_idx / 8 + (w_idx % 8 != 0 ? 1 : 0);
    if (fwrite(wbuff, sizeof(unsigned char), w_size, wfile) < w_size) {
      fprintf(stderr, "Error writing remaining bits to file.\n");
      fclose(rfile);
      return -1;
    }
  }
  // close file
  fclose(rfile);
  return 0;
}

/*
 *
 *
 *
 *
 *
 * Public functions
 *
 *
 *
 *
 *
 * */

double io_read_bytes(Node *pq, char *file_name) {
  FILE *file = fopen(file_name, "rb");
  if (file == NULL) {
    fprintf(stderr, "No se pudo leer el archivo: %s\n", file_name);
    exit(EXIT_FAILURE);
  }
  unsigned char buffer[BUFFER_SIZE];
  double total_bytes = 0;
  size_t bytes_read;
  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
    total_bytes += bytes_read;
    for (size_t i = 0; i < bytes_read; ++i) {
      ++pq[buffer[i]].frequency;
    }
  }
  fclose(file);
  return total_bytes;
}

/*
 * Requires
 * - File descriptor
 * - filename
 * - huffman code
 * - huffman tree
 * Do:
 * 1. Write name
 * 2. Write tree
 * 3. Save position to save file size with long long
 * 4. Write file
 * */
[[nodiscard("Handling error")]]
int io_save_code(FILE *file, char *filename, unsigned char **huff_code,
                 Node *root) {
  // Write name
  int status = 0;
  status = fwrite(filename, sizeof(char), strlen(filename), file);
  if (status < 0) {
    fprintf(stderr, "Error writing filename: %s\n", filename);
    return -1;
  }
  // Write tree
  io_write_huffman_tree(file, root);
  status = io_write_huffman_code(file, huff_code, filename);
  // handle error
  if (status < 0) {
    fprintf(stderr, "Error writing huffman code for file: %s\n", filename);
    return -1;
  }
  return status;
}
