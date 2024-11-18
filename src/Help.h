#pragma once

#include <iostream>

class Help
{
    public:
        static const std::string general(void)
        {
            std::string result;
            char line[100];
            snprintf(line, 100, "%-25s%s\n", "Esc", "Quit");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "F", "Toggle fullscreen");
            result.append(line);
            return result;
        }

        static const std::string allImages(void)
        {
            std::string result;
            char line[100];
            snprintf(line, 100, "%-25s%s\n", "Up, K, Mouse extra2", "Scroll up");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "Down, J, Mouse extra1", "Scroll down");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "Page Up, U", "Scroll up fast");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "Page Down, D", "Scroll down fast");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "Space", "Toggle auto scroll");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "Home, G", "Scroll to top");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "Mouse left", "Elevate image");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "1 - 0", "Set columns to 1 - 10");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "I", "Show info");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "S", "Switch to selected images / all images");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "C", "Clear all selected images");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "A, Mouse middle", "(Un)select image under mouse");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "U", "Toggle sort order");
            result.append(line);
            return result;
        }

        static const std::string singleImage(void)
        {
            std::string result;
            char line[100];
            snprintf(line, 100, "%-25s%s\n", "M, Mouse right", "Go back");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "Mouse left", "Move image");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "Mouse wheel", "Zoom image");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "Space, Mouse extra1", "Next image");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "Backspace, Mouse extra2", "Previous image");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "A, Mouse middle", "(Un)select image");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "O", "Toggle image original size");
            result.append(line);
            return result;
        }

        static const std::string selectedImages(void)
        {
            std::string result;
            char line[100];
            snprintf(line, 100, "%-25s%s\n", "Y", "Copy selected images to 'piv-selected'");
            result.append(line);
            snprintf(line, 100, "%-25s%s\n", "X", "Move selected images to 'piv-selected'");
            result.append(line);
            return result;
        }
};
