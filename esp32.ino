#include <WiFi.h>
#include <WebServer.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>

// Pin Definitions for E-Paper Display
#define EPD_SS 5
#define EPD_DC 17
#define EPD_RST 16
#define EPD_BUSY 4
#define MAX_DISPLAY_BUFFER_SIZE 800

#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
GxEPD2_3C<GxEPD2_290_C90c, MAX_HEIGHT(GxEPD2_290_C90c)>
    display(GxEPD2_290_C90c(EPD_SS, EPD_DC, EPD_RST, EPD_BUSY));

// WiFi Credentials
const char *ssid = "**********";
const char *password = "********";
// line
//  Web server on port 80
WebServer server(80);

// Text line configurations
struct TextLine
{
    String text = "";
    int x = 0;                          // Default X position
    int y = 0;                          // Default Y position
    int fontSize = 9;                   // Default font size (pt)
    uint16_t color = GxEPD_BLACK;       // Default text color
    int borderWidth = 0;                // Border thickness
    int padding = 0;                    // Padding around text inside the border
    uint16_t borderColor = GxEPD_BLACK; // Border color
};

// Three text lines
TextLine textLines[3] = {
    {"Surin Learning Lab", 23, 20, 12, GxEPD_WHITE, 5, 5, GxEPD_WHITE},                                                                                    // Line 1 default
    {"Build + Code + Create                      090-912-7265                            surinlearninglab.com", 2, 50, 9, GxEPD_WHITE, 0, 0, GxEPD_BLACK}, // Line 2 default
    {"6,500 Baht", 150, 120, 12, GxEPD_WHITE, 5, 2, GxEPD_RED}                                                                                             // Line 3 default
};

// Background color
uint16_t backgroundColor = GxEPD_BLACK;

// Function to render the web page
void handleRoot()
{
    String html = "<html><head>"
                  "<style>"
                  "body { font-family: Arial, sans-serif; margin: 20px; }"
                  "input, select { margin: 5px; padding: 5px; }"
                  "</style></head><body>"
                  "<h1>ESP32 E-Paper Display</h1>"
                  "<form action='/updateText' method='POST'>";

    for (int i = 0; i < 3; i++)
    {
        html += "<h3>Text Line " + String(i + 1) + "</h3>";
        html += "<label for='text" + String(i) + "'>Enter text:</label><br>";
        html += "<input type='text' id='text" + String(i) + "' name='text" + String(i) + "' value='" + textLines[i].text + "'><br><br>";
        html += "<label for='x" + String(i) + "'>X Position (0-150):</label><br>";
        html += "<input type='number' id='x" + String(i) + "' name='x" + String(i) + "' value='" + String(textLines[i].x) + "' min='0' max=''><br><br>";
        html += "<label for='y" + String(i) + "'>Y Position (0-296):</label><br>";
        html += "<input type='number' id='y" + String(i) + "' name='y" + String(i) + "' value='" + String(textLines[i].y) + "' min='0' max='296'><br><br>";
        html += "<label for='fontSize" + String(i) + "'>Font Size:</label><br>";
        html += "<select id='fontSize" + String(i) + "' name='fontSize" + String(i) + "'>"
                                                                                      "<option value='9'" +
                (textLines[i].fontSize == 9 ? " selected" : "") + ">9pt</option>"
                                                                  "<option value='12'" +
                (textLines[i].fontSize == 12 ? " selected" : "") + ">12pt</option>"
                                                                   "<option value='18'" +
                (textLines[i].fontSize == 18 ? " selected" : "") + ">18pt</option>"
                                                                   "</select><br><br>";
        html += "<label for='color" + String(i) + "'>Text Color:</label><br>";
        html += "<select id='color" + String(i) + "' name='color" + String(i) + "'>"
                                                                                "<option value='BLACK'" +
                (textLines[i].color == GxEPD_BLACK ? " selected" : "") + ">Black</option>"
                                                                         "<option value='RED'" +
                (textLines[i].color == GxEPD_RED ? " selected" : "") + ">Red</option>"
                                                                       "<option value='WHITE'" +
                (textLines[i].color == GxEPD_WHITE ? " selected" : "") + ">White</option>"
                                                                         "</select><br><br>";
        html += "<label for='borderWidth" + String(i) + "'>Border Thickness (0-10):</label><br>";
        html += "<input type='number' id='borderWidth" + String(i) + "' name='borderWidth" + String(i) + "' value='" + String(textLines[i].borderWidth) + "' min='0' max='10'><br><br>";
        html += "<label for='padding" + String(i) + "'>Padding (0-20):</label><br>";
        html += "<input type='number' id='padding" + String(i) + "' name='padding" + String(i) + "' value='" + String(textLines[i].padding) + "' min='0' max='20'><br><br>";
        html += "<label for='borderColor" + String(i) + "'>Border Color:</label><br>";
        html += "<select id='borderColor" + String(i) + "' name='borderColor" + String(i) + "'>"
                                                                                            "<option value='BLACK'" +
                (textLines[i].borderColor == GxEPD_BLACK ? " selected" : "") + ">Black</option>"
                                                                               "<option value='RED'" +
                (textLines[i].borderColor == GxEPD_RED ? " selected" : "") + ">Red</option>"
                                                                             "<option value='WHITE'" +
                (textLines[i].borderColor == GxEPD_WHITE ? " selected" : "") + ">White</option>"
                                                                               "</select><br><br>";
    }

    html += "<h3>Global Settings</h3>"
            "<label for='bgColor'>Select background color:</label><br>"
            "<select id='bgColor' name='bgColor'>"
            "<option value='WHITE'>White</option>"
            "<option value='BLACK'>Black</option>"
            "<option value='RED'>Red</option>"
            "</select><br><br>"
            "<input type='submit' value='Update Display'>"
            "</form></body></html>";
    server.send(200, "text/html", html);
}

// Function to handle text updates
void handleUpdateText()
{
    for (int i = 0; i < 3; i++)
    {
        if (server.hasArg("text" + String(i)))
        {
            textLines[i].text = server.arg("text" + String(i));
        }
        if (server.hasArg("x" + String(i)))
        {
            textLines[i].x = server.arg("x" + String(i)).toInt();
        }
        if (server.hasArg("y" + String(i)))
        {
            textLines[i].y = server.arg("y" + String(i)).toInt();
        }
        if (server.hasArg("fontSize" + String(i)))
        {
            textLines[i].fontSize = server.arg("fontSize" + String(i)).toInt();
        }
        if (server.hasArg("color" + String(i)))
        {
            String colorArg = server.arg("color" + String(i));
            if (colorArg == "BLACK")
                textLines[i].color = GxEPD_BLACK;
            else if (colorArg == "RED")
                textLines[i].color = GxEPD_RED;
            else if (colorArg == "WHITE")
                textLines[i].color = GxEPD_WHITE;
        }
        if (server.hasArg("borderWidth" + String(i)))
        {
            textLines[i].borderWidth = server.arg("borderWidth" + String(i)).toInt();
        }
        if (server.hasArg("padding" + String(i)))
        {
            textLines[i].padding = server.arg("padding" + String(i)).toInt();
        }
        if (server.hasArg("borderColor" + String(i)))
        {
            String borderColorArg = server.arg("borderColor" + String(i));
            if (borderColorArg == "BLACK")
                textLines[i].borderColor = GxEPD_BLACK;
            else if (borderColorArg == "RED")
                textLines[i].borderColor = GxEPD_RED;
            else if (borderColorArg == "WHITE")
                textLines[i].borderColor = GxEPD_WHITE;
        }
    }

    // Update global background color
    if (server.hasArg("bgColor"))
    {
        String bgColorArg = server.arg("bgColor");
        if (bgColorArg == "WHITE")
            backgroundColor = GxEPD_WHITE;
        else if (bgColorArg == "BLACK")
            backgroundColor = GxEPD_BLACK;
        else if (bgColorArg == "RED")
            backgroundColor = GxEPD_RED;
    }

    // Update the display
    updateDisplay();
    server.send(200, "text/html", "<html><body><h1>Display Updated!</h1><a href='/'>Go Back</a></body></html>");
}

// Function to update the e-paper display
void updateDisplay()
{
    display.setRotation(1);
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(backgroundColor);
        for (int i = 0; i < 3; i++)
        {
            // Set font size
            if (textLines[i].fontSize == 9)
            {
                display.setFont(&FreeMonoBold9pt7b);
            }
            else if (textLines[i].fontSize == 12)
            {
                display.setFont(&FreeMonoBold12pt7b);
            }
            else if (textLines[i].fontSize == 18)
            {
                display.setFont(&FreeMonoBold18pt7b);
            }

            // Measure text bounding box
            int16_t tbx, tby;
            uint16_t tbw, tbh;
            display.getTextBounds(textLines[i].text.c_str(), textLines[i].x, textLines[i].y, &tbx, &tby, &tbw, &tbh);

            // Adjust bounding box for padding
            tbx -= textLines[i].padding;
            tby -= textLines[i].padding;
            tbw += 2 * textLines[i].padding;
            tbh += 2 * textLines[i].padding;

            // Draw border box with thickness
            for (int b = 0; b < textLines[i].borderWidth; b++)
            {
                display.drawRect(
                    tbx - b,
                    tby - b,
                    tbw + 2 * b,
                    tbh + 2 * b,
                    textLines[i].borderColor);
            }

            // Draw text
            display.setTextColor(textLines[i].color);
            display.setCursor(textLines[i].x, textLines[i].y);
            display.print(textLines[i].text);
        }
    } while (display.nextPage());
}

void setup()
{
    // Initialize serial and display
    Serial.begin(115200);
    display.init(115200);

    // Connect to WiFi
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Display IP address on the e-paper
    updateDisplay();

    // Initialize web server routes
    server.on("/", handleRoot);
    server.on("/updateText", HTTP_POST, handleUpdateText);
    server.begin();
    Serial.println("Web server started.");
}

void loop()
{
    server.handleClient();
}
