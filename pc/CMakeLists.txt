
#############
# Flight Deck

add_executable(flight-deck
  src/flight_deck.cpp
)

target_link_libraries(flight-deck PUBLIC
  rcplane_common_io
)
