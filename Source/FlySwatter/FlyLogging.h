#pragma once


#define PRINT_MESSAGE(Color, Format, ...) \
if (GEngine) \
{ \
FString Message = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Message); \
}
