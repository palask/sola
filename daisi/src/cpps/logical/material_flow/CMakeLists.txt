add_library(daisi_cpps_logical_material_flow_material_flow_logical_agent STATIC)
target_sources(daisi_cpps_logical_material_flow_material_flow_logical_agent
    PRIVATE
    material_flow_logical_agent.cpp
    material_flow_logical_agent.h
)
target_link_libraries(daisi_cpps_logical_material_flow_material_flow_logical_agent
    PUBLIC
    daisi_cpps_logical_logical_agent
    daisi_cpps_logical_algorithms_disposition_iterated_auction_disposition_initiator
    daisi_cpps_logical_algorithms_disposition_round_robin_initiator
    daisi_material_flow_model_material_flow
    PRIVATE
    solanet_uuid
    solanet_uuid_generator_sim
    daisi_cpps_logical_algorithms_algorithm_config
    daisi_cpps_logical_message_material_flow_update
)
