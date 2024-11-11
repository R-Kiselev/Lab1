#include "../../include/SocialStatus/SocialStatusService.h"

SocialStatusService::SocialStatusService(SocialStatusRepository *social_status_repository)
        : social_status_repository_(social_status_repository)
{
    validation_service = std::make_unique<ValidationService>();
}
void SocialStatusService::add(SocialStatus* social_status) const {
    validation_service->validate_name(social_status->get_name());
    social_status_repository_->add(social_status);
}
void SocialStatusService::remove(int id) {
    validation_service->validate_id(id);
    auto social_status = social_status_repository_->get_by_id(id);
    social_status_repository_->remove(social_status->get_id());
}
void SocialStatusService::update(const int id, SocialStatus* new_social_status) const {
    validation_service->validate_id(id);
    validation_service->validate_name(new_social_status->get_name());
    auto social_status = social_status_repository_->get_by_id(id);
    social_status->set_name(new_social_status->get_name());
    social_status_repository_->update(social_status.get());
}

std::unique_ptr<SocialStatus> SocialStatusService::get_by_id(const int id) const
{
    validation_service->validate_id(id);
    return social_status_repository_->get_by_id(id);
}
std::vector<std::unique_ptr<SocialStatus>> SocialStatusService::get_all() const
{
    return social_status_repository_->get_all();
}
void SocialStatusService::display_all() const {
    auto social_statuses = social_status_repository_->get_all();
    if (social_statuses.empty()) {
        std::cout << "No social_statuses available." << std::endl;
    } else {
        for (const auto& social_status : social_statuses) {
            std::cout << "SocialStatus ID: " << social_status->get_id() << ", Name: " << social_status->get_name() << std::endl;
        }
    }
}
bool SocialStatusService::exists(const int id) const {
    validation_service->validate_id(id);
    return social_status_repository_->exists(id);
}
