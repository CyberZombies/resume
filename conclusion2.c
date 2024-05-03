#include <windows.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>
#include <math.h>

// void Keyboard_layout(int layout) { //1-русский, 2 - английский
// if (layout == 1){
// PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 1, 0x04190419);
//  printf("rus\n");
// }else{
//   PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 1, 0x04090409);
//  printf("ang\n");
// }
// }
int waitingForInputField()
{
  HWND activeWindow = GetFocus();
  CURSORINFO cursorInfo;
  cursorInfo.cbSize = sizeof(CURSORINFO);

  if (GetCursorInfo(&cursorInfo))
  {
    if (cursorInfo.flags & CURSOR_SHOWING && cursorInfo.hCursor == LoadCursor(NULL, IDC_IBEAM))
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return 0;
  }
}
int checkingForLayoutLanguage()
{
  HWND foregroundWindow = GetForegroundWindow();
  DWORD threadId = GetWindowThreadProcessId(foregroundWindow, NULL);
  HKL keyboardLayout = GetKeyboardLayout(threadId);
  int keyboardLayoutId = (int)keyboardLayout & 0xFFFF; // Получение нижних 16 бит
  Sleep(30);
  //   printf("Iz=0x%04X\n", keyboardLayoutId);
  int cursor_state = 0;
  do
  {
    cursor_state = 0;
    cursor_state = waitingForInputField();
  } while (cursor_state == 0);
  return keyboardLayoutId;
}
void keyboard(char password[], int i, int registr)
{
  INPUT input;
  int keyboardLayoutId = 0, byteCombinationToBeChecked = 0, byteCycleIndex, raisingTenToAPower = 0, shift = 0;
  input.type = INPUT_KEYBOARD;
  char symbol_now = password[i];
  int Russian_index = (char)symbol_now;
  int keyboard_index = (char)symbol_now;
  for (int i = sizeof(symbol_now) * 8 - 1; i >= 0; i--)
  {
    raisingTenToAPower = pow(10, byteCycleIndex);
    byteCombinationToBeChecked = byteCombinationToBeChecked + ((symbol_now >> i) & 1) * raisingTenToAPower;
    byteCycleIndex++;
  }
  if ((Russian_index <= -65) && (Russian_index >= -128) || (Russian_index == -30) || (password[i] == '?') || (password[i] == '/') || (password[i] == '\"'))
  {
    do
    {
      PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 1, 0x04190419);
      keyboardLayoutId = checkingForLayoutLanguage();
    } while (keyboardLayoutId != 1049);
    // printf("rus=%d\n",keyboardLayoutId);
  }
  else
  {
    do
    {
      PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 1, 0x04090409);
      keyboardLayoutId = checkingForLayoutLanguage();
    } while (keyboardLayoutId != 1033);
    // printf("ang=%d\n",keyboardLayoutId);
  }
  if ((password[i] >= 97) && (password[i] <= 122)) // инглишь
  {
    int keyboard_index = (char)symbol_now - 32;
    input.ki.wVk = keyboard_index;
  }
  else if ((password[i] >= 65) && (password[i] <= 90)) // большой инглишь
  {
    input.ki.wVk = keyboard_index;
    shift = 1;
  }
  else if ((password[i] >= '0') && (password[i] <= '9'))
  {
    input.ki.wVk = keyboard_index;
  }
  else if (((Russian_index <= -65) && (Russian_index >= -128) && (byteCombinationToBeChecked != 1101001)))
  {
    switch (Russian_index)
    {
    case -65:
      input.ki.wVk = 0x47;
      break;
    case -66:
      input.ki.wVk = 0x4A;
      break;
    case -67:
      input.ki.wVk = 0x59;
      break;
    case -68:
      input.ki.wVk = 0x56;
      break;
    case -69:
      input.ki.wVk = 0x4B;
      break;
    case -70:
      input.ki.wVk = 0x52;
      break;
    case -71:
      input.ki.wVk = 0x51;
      break;
    case -72:
      input.ki.wVk = 0x42;
      break;
    case -73:
      input.ki.wVk = 0x50;
      break;
    case -74:
      input.ki.wVk = 0xBA;
      break;
    case -75:
      input.ki.wVk = 0x54;
      break;
    case -76:
      input.ki.wVk = 0x4C;
      break;
    case -77:
      input.ki.wVk = 0x55;
      break;
    case -78:
      input.ki.wVk = 0x44;
      break;
    case -79:
      input.ki.wVk = 0xBC;
      break;
    case -80:
      input.ki.wVk = 0x46;
      break;
    case -111:
      if (registr == 0)
      {
        input.ki.wVk = 0xC0;
      }
      else
      {
        input.ki.wVk = 0xBC;
        shift = 1;
      }
      break;
    case -113:
      input.ki.wVk = 0x5A;
      break;
    case -114:
      input.ki.wVk = 0xBE;
      break;
    case -115:
      input.ki.wVk = 0xDE;
      break;
    case -116:
      input.ki.wVk = 0x4D;
      break;
    case -117:
      input.ki.wVk = 0x53;
      break;
    case -118:
      input.ki.wVk = 0xDD;
      break;
    case -119:
      input.ki.wVk = 0x4F;
      break;
    case -120:
      input.ki.wVk = 0x49;
      break;
    case -121:
      input.ki.wVk = 0x58;
      break;
    case -122:
      input.ki.wVk = 0x57;
      break;
    case -123:
      input.ki.wVk = 0xDB;
      break;
    case -124:
      input.ki.wVk = 0x41;
      break;
    case -125:
      input.ki.wVk = 0x45;
      break;
    case -126:
      input.ki.wVk = 0x4E;
      break;
    case -127:
      if (registr == 0)
      {
        input.ki.wVk = 0x43;
      }
      else
      {
        input.ki.wVk = 0xC0;
        shift = 1;
      }
      break;
    case -128:
      input.ki.wVk = 0x48;
      break;
    case -81:
      input.ki.wVk = 0x5A;
      shift = 1;
      break;
    case -82:
      input.ki.wVk = 0xBE;
      shift = 1;
      break;
    case -83:
      input.ki.wVk = 0xDE;
      shift = 1;
      break;
    case -84:
      input.ki.wVk = 0x4D;
      shift = 1;
      break;
    case -85:
      input.ki.wVk = 0x53;
      shift = 1;
      break;
    case -86:
      input.ki.wVk = 0xDD;
      shift = 1;
      break;
    case -87:
      input.ki.wVk = 0x4F;
      shift = 1;
      break;
    case -88:
      input.ki.wVk = 0x49;
      shift = 1;
      break;
    case -89:
      input.ki.wVk = 0x58;
      shift = 1;
      break;
    case -90:
      input.ki.wVk = 0x57;
      shift = 1;
      break;
    case -91:
      input.ki.wVk = 0xDB;
      shift = 1;
      break;
    case -92:
      input.ki.wVk = 0x41;
      shift = 1;
      break;
    case -93:
      input.ki.wVk = 0x45;
      shift = 1;
      break;
    case -94:
      input.ki.wVk = 0x4E;
      shift = 1;
      break;
    case -95:
      input.ki.wVk = 0x43;
      shift = 1;
      break;
    case -96:
      input.ki.wVk = 0x48;
      shift = 1;
      break;
    case -97:
      input.ki.wVk = 0x47;
      shift = 1;
      break;
    case -98:
      input.ki.wVk = 0x4A;
      shift = 1;
      break;
    case -99:
      input.ki.wVk = 0x59;
      shift = 1;
      break;
    case -100:
      input.ki.wVk = 0x56;
      shift = 1;
      break;
    case -101:
      input.ki.wVk = 0x4B;
      shift = 1;
      break;
    case -102:
      input.ki.wVk = 0x52;
      shift = 1;
      break;
    case -103:
      input.ki.wVk = 0x51;
      shift = 1;
      break;
    case -104:
      input.ki.wVk = 0x42;
      shift = 1;
      break;
    case -105:
      input.ki.wVk = 0x50;
      shift = 1;
      break;
    case -106:
      input.ki.wVk = 0xBA;
      shift = 1;
      break;
    case -107:
      input.ki.wVk = 0x54;
      shift = 1;
      break;
    case -108:
      input.ki.wVk = 0x4C;
      shift = 1;
      break;
    case -109:
      input.ki.wVk = 0x55;
      shift = 1;
      break;
    case -110:
      input.ki.wVk = 0x44;
      shift = 1;
      break;
    case -112:
      input.ki.wVk = 0x46;
      shift = 1;
      break;
    }
  }
  else
    switch (password[i])
    {
    case ' ':
      input.ki.wVk = 0x20;
      break;
    case '.':
      input.ki.wVk = 0xBE;
      break;
    case '\'':
      input.ki.wVk = 0xDE;
      break;
    case '\\':
      input.ki.wVk = 0xDC;
      break;
    case ',':
      input.ki.wVk = 0xBC;
      break;
    case ';':
      input.ki.wVk = 0xBA;
      break;
    case '`':
      input.ki.wVk = 0xC0;
      break;
    case '-':
      input.ki.wVk = 0xBD;
      break;
    case '=':
      input.ki.wVk = 0xBB;
      break;
    case '[':
      input.ki.wVk = 0xDB;
      break;
    case ']':
      input.ki.wVk = 0xDD;
      break;
    }

  if ((password[i] == '!') || (password[i] == '@') || (password[i] == '#') ||
      (password[i] == '$') || (password[i] == '%') || (password[i] == '^') ||
      (password[i] == '&') || (password[i] == '*') || (password[i] == '(') ||
      (password[i] == ')') || (password[i] == '_') || (password[i] == '+') ||
      (password[i] == ':') || (password[i] == '{') || (password[i] == '}') ||
      (password[i] == '|') || (password[i] == '/') || (password[i] == '?') ||
      (password[i] == '~') || (password[i] == '<') || (password[i] == '>') ||
      (password[i] == '\"') || (byteCombinationToBeChecked == 1101001))
  {
    switch (password[i])
    {
    case '!':
      input.ki.wVk = 0x31;
      break;
    case '@':
      input.ki.wVk = 0x32;
      break;
    case '#':
      input.ki.wVk = 0x33;
      break;
    case '$':
      input.ki.wVk = 0x34;
      break;
    case '%':
      input.ki.wVk = 0x35;
      break;
    case '^':
      input.ki.wVk = 0x36;
      break;
    case '&':
      input.ki.wVk = 0x37;
      break;
    case '*':
      input.ki.wVk = 0x38;
      break;
    case ':':
      input.ki.wVk = 0xBA;
      break;
    case '(':
      input.ki.wVk = 0x39;
      break;
    case ')':
      input.ki.wVk = 0x30;
      break;
    case '_':
      input.ki.wVk = 0xBD;
      break;
    case '+':
      input.ki.wVk = 0xBB;
      break;
    case '{':
      input.ki.wVk = 0xDB;
      break;
    case '}':
      input.ki.wVk = 0xDD;
      break;
    case '|':
      input.ki.wVk = 0xDC;
      break;
    case '/':
      input.ki.wVk = 0xDC;
      break;
    case '?':
      input.ki.wVk = 0x37;
      break;
    case '~':
      input.ki.wVk = 0xC0;
      break;
    case '<':
      input.ki.wVk = 0xBC;
      break;
    case '>':
      input.ki.wVk = 0xBE;
      break;
    case '\"':
      input.ki.wVk = 0x32;
      break;
    default:
      input.ki.wVk = 0x33;
      break; // №-3 байта,костыль
    }
    shift = 1;
  }

  if (shift == 1)
  {
    keybd_event(VK_SHIFT, 0, 0, 0);
  }
  keybd_event(input.ki.wVk, 0, 0, 0);
  keybd_event(input.ki.wVk, 0, KEYEVENTF_KEYUP, 0);
  if (shift == 1)
  {
    keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
  }
}

int zapysk(char word1[])
{
  // void main(){
  // Sleep(1000);
  char word[] = "2";
  word[0] = word1;

  //  word[1] = "5";

  for (int i = 0; i <= strlen(word) - 1; i++)
  {
    int registr = 0;
    char symbol_now = word[i];

    //  printf("sim=%c\n", symbol_now);
    int index = (char)symbol_now;
    //  printf("reg=%d\n", index);
    if (((index == -48) || (index == -47)))
    {
      i++;
      if (index == -48)
      {
        registr = 1;
      }
    }

    if ((index != 0))
    {
      keyboard(word, i, registr);
    }
    if ((index == -30))
    {
      i = i + 2;
    }
  }
  return 0;
}