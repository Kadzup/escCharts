#include "BMP.h"

void BMPFormat::BMP::Read(const char* fileName) {
    std::ifstream inp{ fileName, std::ios_base::binary };
    if (inp) {
        inp.read((char*)&fileHeader, sizeof(fileHeader));
        if (fileHeader.file_type != 0x4D42) {
            throw std::runtime_error("Error! Unrecognized file format.");
        }
        inp.read((char*)&bmpInfoHeader, sizeof(bmpInfoHeader));

        // The BMPColorHeader is used only for transparent images
        if (bmpInfoHeader.bit_count == 32) {
            // Check if the file has bit mask color information
            if (bmpInfoHeader.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
                inp.read((char*)&bmpColorHeader, sizeof(bmpColorHeader));
                // Check if the pixel data is stored as BGRA and if the color space type is sRGB
                CheckColorHeader(bmpColorHeader);
            }
            else {
                std::cerr << "Error! The file \"" << fileName << "\" does not seem to contain bit mask information\n";
                throw std::runtime_error("Error! Unrecognized file format.");
            }
        }

        // Jump to the pixel data location
        inp.seekg(fileHeader.offset_data, inp.beg);

        // Adjust the header fields for output.
        // Some editors will put extra info in the image file, we only save the headers and the data.
        if (bmpInfoHeader.bit_count == 32) {
            bmpInfoHeader.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            fileHeader.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        }
        else {
            bmpInfoHeader.size = sizeof(BMPInfoHeader);
            fileHeader.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
        }
        fileHeader.file_size = fileHeader.offset_data;

        if (bmpInfoHeader.height < 0) {
            throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
        }

        data.resize(bmpInfoHeader.width * bmpInfoHeader.height * bmpInfoHeader.bit_count / 8);

        // Here we check if we need to take into account row padding
        if (bmpInfoHeader.width % 4 == 0) {
            inp.read((char*)data.data(), data.size());
            fileHeader.file_size += static_cast<uint32_t>(data.size());
        }
        else {
            rowStride = bmpInfoHeader.width * bmpInfoHeader.bit_count / 8;
            uint32_t newStride = MakeStrideAligned(4);
            std::vector<uint8_t> padding_row(newStride - rowStride);

            for (int y = 0; y < bmpInfoHeader.height; ++y) {
                inp.read((char*)(data.data() + rowStride * y), rowStride);
                inp.read((char*)padding_row.data(), padding_row.size());
            }
            fileHeader.file_size += static_cast<uint32_t>(data.size()) + bmpInfoHeader.height * static_cast<uint32_t>(padding_row.size());
        }
    }
    else {
        std::ofstream outfile(fileName);
        Read(fileName);
        //throw std::runtime_error("Unable to open the input image file.");
    }
}

void BMPFormat::BMP::Write(const char* fileName) {
    std::ofstream of{ fileName, std::ios_base::binary };
    if (of) {
        if (bmpInfoHeader.bit_count == 32) {
            WriteHeadersAndData(of);
        }
        else if (bmpInfoHeader.bit_count == 24) {
            if (bmpInfoHeader.width % 4 == 0) {
                WriteHeadersAndData(of);
            }
            else {
                uint32_t newStride = MakeStrideAligned(4);
                std::vector<uint8_t> padding_row(newStride - rowStride);

                WriteHeaders(of);

                for (int y = 0; y < bmpInfoHeader.height; ++y) {
                    of.write((const char*)(data.data() + rowStride * y), rowStride);
                    of.write((const char*)padding_row.data(), padding_row.size());
                }
            }
        }
        else {
            throw std::runtime_error("The program can treat only 24 or 32 bits per pixel BMP files");
        }
    }
    else {
        throw std::runtime_error("Unable to open the output image file.");
    }
}

void BMPFormat::BMP::FillRegion(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint8_t B, uint8_t G, uint8_t R, uint8_t A) {
    if (x0 + w > (uint32_t)bmpInfoHeader.width || y0 + h > (uint32_t)bmpInfoHeader.height) {
        throw std::runtime_error("The region does not fit in the image!");
    }

    uint32_t channels = bmpInfoHeader.bit_count / 8;
    for (uint32_t y = y0; y < y0 + h; ++y) {
        for (uint32_t x = x0; x < x0 + w; ++x) {
            data[channels * (y * bmpInfoHeader.width + x) + 0] = B;
            data[channels * (y * bmpInfoHeader.width + x) + 1] = G;
            data[channels * (y * bmpInfoHeader.width + x) + 2] = R;
            if (channels == 4) {
                data[channels * (y * bmpInfoHeader.width + x) + 3] = A;
            }
        }
    }
}

void BMPFormat::BMP::SetPixel(uint32_t x0, uint32_t y0, uint8_t B, uint8_t G, uint8_t R, uint8_t A) {
    if (x0 > (uint32_t)bmpInfoHeader.width || y0 > (uint32_t)bmpInfoHeader.height) {
        throw std::runtime_error("The point is outside the image boundaries!");
    }

    uint32_t channels = bmpInfoHeader.bit_count / 8;
    data[channels * (y0 * bmpInfoHeader.width + x0) + 0] = B;
    data[channels * (y0 * bmpInfoHeader.width + x0) + 1] = G;
    data[channels * (y0 * bmpInfoHeader.width + x0) + 2] = R;
    if (channels == 4) {
        data[channels * (y0 * bmpInfoHeader.width + x0) + 3] = A;
    }
}

void BMPFormat::BMP::DrawRectangle(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h,
    uint8_t B, uint8_t G, uint8_t R, uint8_t A, uint8_t line_w) {
    if (x0 + w > (uint32_t)bmpInfoHeader.width || y0 + h > (uint32_t)bmpInfoHeader.height) {
        throw std::runtime_error("The rectangle does not fit in the image!");
    }

    FillRegion(x0, y0, w, line_w, B, G, R, A);                                             // top line
    FillRegion(x0, (y0 + h - line_w), w, line_w, B, G, R, A);                              // bottom line
    FillRegion((x0 + w - line_w), (y0 + line_w), line_w, (h - (2 * line_w)), B, G, R, A);  // right line
    FillRegion(x0, (y0 + line_w), line_w, (h - (2 * line_w)), B, G, R, A);                 // left line
}

uint32_t BMPFormat::BMP::MakeStrideAligned(uint32_t alignStride) {
    uint32_t newStride = rowStride;
    while (newStride % alignStride != 0) {
        newStride++;
    }
    return newStride;
}

void BMPFormat::BMP::CheckColorHeader(BMPColorHeader& bmpColorHeader) {
    BMPColorHeader expectedColorHeader;
    if (expectedColorHeader.red_mask != bmpColorHeader.red_mask ||
        expectedColorHeader.blue_mask != bmpColorHeader.blue_mask ||
        expectedColorHeader.green_mask != bmpColorHeader.green_mask ||
        expectedColorHeader.alpha_mask != bmpColorHeader.alpha_mask) {
        throw std::runtime_error("Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
    }
    if (expectedColorHeader.color_space_type != bmpColorHeader.color_space_type) {
        throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
    }
}

void BMPFormat::BMP::WriteHeadersAndData(std::ofstream& of) {
    WriteHeaders(of);
    of.write((const char*)data.data(), data.size());
}

void BMPFormat::BMP::WriteHeaders(std::ofstream& of) {
    of.write((const char*)&fileHeader, sizeof(fileHeader));
    of.write((const char*)&bmpInfoHeader, sizeof(bmpInfoHeader));
    if (bmpInfoHeader.bit_count == 32) {
        of.write((const char*)&bmpColorHeader, sizeof(bmpColorHeader));
    }
}