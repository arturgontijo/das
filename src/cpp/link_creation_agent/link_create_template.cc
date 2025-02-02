#include "link_create_template.h"

#include <iostream>
#include <stdexcept>

using namespace link_creation_agent;

CustomField::CustomField(std::vector<std::string> args) {
    if (args[0] != "CUSTOM_FIELD")
        throw std::invalid_argument("Can not create Custom Field: Invalid arguments");

    int cursor = 0;
    int custom_field_size = std::stoi(args[2]);
    std::string custom_field_name = args[1];
    this->name = custom_field_name;
    cursor += 3;
    while (cursor < args.size()) {

        if (args[cursor] == "CUSTOM_FIELD") {
            std::vector<std::string> custom_field_args;
            int sub_custom_field_size = std::stoi(args[cursor + 2]);
            std::string sub_custom_field_name = args[cursor + 1];
            custom_field_args.push_back(args[cursor]);      // CUSTOM_FIELD
            custom_field_args.push_back(args[cursor + 1]);  // field name
            custom_field_args.push_back(args[cursor + 2]);  // field size
            cursor += 3;
            while (cursor < args.size()) {
                if (sub_custom_field_size == 0) {
                    break;
                }

                custom_field_args.push_back(args[cursor]);
                if (args[cursor] == "CUSTOM_FIELD") {
                    sub_custom_field_size += std::stoi(args[cursor + 2]);
                    custom_field_args.push_back(args[cursor + 1]);  // field name
                    custom_field_args.push_back(args[cursor + 2]);  // field size
                    cursor += 3;
                    sub_custom_field_size--;
                } else {
                    custom_field_args.push_back(args[cursor + 1]);
                    cursor += 2;
                    sub_custom_field_size--;
                }
            }
            CustomField custom_field = CustomField(custom_field_args);
            this->values.push_back(
                std::make_tuple(sub_custom_field_name, std::make_shared<CustomField>(custom_field)));
        } else {
            this->values.push_back(std::make_tuple(args[cursor], args[cursor + 1]));
            cursor += 2;
        }
    }
}

CustomField::~CustomField() {}

std::string CustomField::get_name() { return this->name; }

std::vector<std::tuple<std::string, CustomFieldTypes>> CustomField::get_values() { return this->values; }

std::vector<std::string> parse_sub_custom_field(std::vector<std::string> link_template, size_t& cursor) {
    std::vector<std::string> custom_field_args;
    int custom_field_size = std::stoi(link_template[cursor + 2]);
    custom_field_args.push_back(link_template[cursor]);      // CUSTOM_FIELD
    custom_field_args.push_back(link_template[cursor + 1]);  // field name
    custom_field_args.push_back(link_template[cursor + 2]);  // field size
    cursor += 3;
    while (cursor < link_template.size()) {
        if (custom_field_size == 0) {
            break;
        }
        custom_field_args.push_back(link_template[cursor]);
        if (link_template[cursor] == "CUSTOM_FIELD") {
            custom_field_args.push_back(link_template[cursor + 1]);
            custom_field_args.push_back(link_template[cursor + 2]);
            custom_field_size += std::stoi(link_template[cursor + 2]);
            custom_field_size--;
            cursor += 3;
        } else {
            custom_field_args.push_back(link_template[cursor + 1]);
            custom_field_size--;
            cursor += 2;
        }
    }
    return custom_field_args;
}

std::vector<std::string> parse_sub_link_template(std::vector<std::string> link_template,
                                                 size_t& cursor) {
    int sub_link_template_size = std::stoi(link_template[cursor + 2]);
    int sub_link_custom_field_size = std::stoi(link_template[cursor + 3]);
    int custom_field_value_size = 0;
    std::vector<std::string> sub_link_template;
    int current_ptr = 0;                                     // link create default size
    sub_link_template.push_back(link_template[cursor]);      // LINK_CREATE
    sub_link_template.push_back(link_template[cursor + 1]);  // link type
    sub_link_template.push_back(link_template[cursor + 2]);  // link size
    sub_link_template.push_back(link_template[cursor + 3]);  // custom field size
    cursor += 4;
    while (cursor < link_template.size()) {
        if (sub_link_template_size == 0 && current_ptr == 0 && sub_link_custom_field_size == 0 &&
            custom_field_value_size == 0) {
            break;
        }
        sub_link_template.push_back(link_template[cursor]);

        if (link_template[cursor] == "NODE") {
            current_ptr = 3;
            sub_link_template_size--;
        }
        if (link_template[cursor] == "VARIABLE") {
            current_ptr = 2;
            sub_link_template_size--;
        }
        if (link_template[cursor] == "LINK_CREATE") {
            current_ptr = 4;
            sub_link_template_size += std::stoi(link_template[cursor + 2]);
            sub_link_custom_field_size = std::stoi(link_template[cursor + 3]);
            sub_link_template_size--;
        }
        if (sub_link_custom_field_size > 0 && link_template[cursor] == "CUSTOM_FIELD") {
            current_ptr = 3 + (std::stoi(link_template[cursor + 2]) * 2);
            sub_link_custom_field_size--;
        }

        current_ptr--;
        cursor++;
    }
    return sub_link_template;
}

LinkCreateTemplate::LinkCreateTemplate(std::vector<std::string> link_template) {
    int stating_pos = 0;
    if (link_template[0] == "LINK_CREATE") {
        stating_pos = 1;
    }

    this->link_type = link_template[stating_pos];
    std::size_t cursor = stating_pos;
    std::size_t link_template_size = std::stoi(link_template[stating_pos + 1]);
    std::size_t custom_field_size = std::stoi(link_template[stating_pos + 2]);
    while (cursor < link_template.size()) {
        if (link_template[cursor] == "NODE") {
            Node node;
            node.type = link_template[cursor + 1];
            node.value = link_template[cursor + 2];
            this->targets.push_back(node);
            cursor += 2;
        } else if (link_template[cursor] == "VARIABLE") {
            Variable var;
            var.name = link_template[cursor + 1];
            this->targets.push_back(var);
            cursor += 1;
        } else if (link_template[cursor] == "LINK_CREATE") {
            std::vector<std::string> sub_link_template = parse_sub_link_template(link_template, cursor);
            LinkCreateTemplateTypes sub_link = std::make_shared<LinkCreateTemplate>(sub_link_template);
            this->targets.push_back(sub_link);
        } else if (link_template[cursor] == "CUSTOM_FIELD") {
            std::vector<std::string> sub_custom_field = parse_sub_custom_field(link_template, cursor);
            CustomField custom_field = CustomField(sub_custom_field);
            this->custom_fields.push_back(custom_field);
            cursor++;
        } else {
            cursor++;
        }
    }
}

LinkCreateTemplate::~LinkCreateTemplate() {}

std::string LinkCreateTemplate::get_link_type() { return this->link_type; }

std::vector<LinkCreateTemplateTypes> LinkCreateTemplate::get_targets() { return this->targets; }

std::string LinkCreateTemplate::to_string() {
    std::string link_template = "LINK_CREATE " + this->link_type + " " +
                                std::to_string(this->targets.size()) + " " +
                                std::to_string(this->custom_fields.size()) + " ";
    for (auto target : this->targets) {
        if (std::holds_alternative<Node>(target)) {
            Node node = std::get<Node>(target);
            link_template += "NODE " + node.type + " " + node.value;
        } else if (std::holds_alternative<Variable>(target)) {
            Variable var = std::get<Variable>(target);
            link_template += "VARIABLE " + var.name;
        } else if (std::holds_alternative<std::shared_ptr<LinkCreateTemplate>>(target)) {
            std::shared_ptr<LinkCreateTemplate> sub_link =
                std::get<std::shared_ptr<LinkCreateTemplate>>(target);
            link_template += sub_link->to_string();
        }
        link_template += " ";
    }
    for (auto custom_field : this->custom_fields) {
        link_template += custom_field.to_string();
    }
    link_template = link_template.substr(0, link_template.size() - 1);
    return link_template;
}

std::string CustomField::to_string() {
    std::string custom_field =
        "CUSTOM_FIELD " + this->name + " " + std::to_string(this->values.size()) + " ";
    for (auto value : this->values) {
        CustomFieldTypes field_value = std::get<1>(value);
        if (std::holds_alternative<std::string>(field_value)) {
            custom_field += std::get<0>(value) + " ";
            custom_field += std::get<std::string>(field_value) + " ";
        } else {
            std::shared_ptr<CustomField> sub_custom_field =
                std::get<std::shared_ptr<CustomField>>(field_value);
            custom_field += sub_custom_field->to_string();
        }
    }
    return custom_field;
}